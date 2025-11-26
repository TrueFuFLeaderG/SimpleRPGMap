#include "scene.h"

#include <MapItem.h>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <qgraphicssceneevent.h>
#include <qgraphicsview.h>

Scene::Scene(const QString &path):m_path(path)
{
    m_fogOfWarBrush=QBrush(QPixmap("./fogofwar.png"));
    m_curser=new QGraphicsEllipseItem(-8,-8,16,16);
    (new QGraphicsEllipseItem(-16,-16,32,32,m_curser))->setPen(QPen(QColor(255,0,0,200),5,Qt::DashLine));
    addItem(m_linear);
    addItem(m_viewportItem);
    m_curser->setAcceptedMouseButtons(Qt::NoButton);
    m_curser->setBrush(QColor(255,0,0,200));
    m_curser->setPen(Qt::NoPen);
    addItem(m_curser);
    m_curser->setZValue(10000);
    m_linear->setZValue(10000);
    m_viewportItem->setZValue(9000);
    m_curser->hide();
    m_linear->hide();
    m_grid=new QGraphicsPixmapItem;
    addItem(m_grid);
    m_grid->setZValue(1);
    if(path!="")
    {
        QPixmap pixmap=QPixmap(path).scaled(QSize(1920,1080),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        m_background=addPixmap(pixmap);
        QPainter painter;
        m_baseFogOfWar=QPixmap(pixmap.size());
        m_grid->setPixmap(QPixmap(pixmap.size()));

        m_baseFogOfWar.fill( Qt::transparent);

        painter.begin(&m_baseFogOfWar);
        painter.fillRect(0,0,m_baseFogOfWar.width(),m_baseFogOfWar.height(),m_fogOfWarBrush);
        painter.end();

        m_fogOfWar=new QGraphicsPixmapItem(m_baseFogOfWar);
        m_fogOfWar->setOpacity(m_fogOfWarOpaque);
        addItem(m_fogOfWar);

        m_viewportItem->setRect(0,0,1920,1080);
    }
    else
    {
        m_viewportItem->hide();
        m_baseFogOfWar=QPixmap(16,16);
        m_baseFogOfWar.fill( Qt::transparent);
        m_fogOfWar=new QGraphicsPixmapItem(m_baseFogOfWar);
        m_fogOfWar->setZValue(1000);
        addItem(m_fogOfWar);
        updateFogOfWar();
        m_background=addPixmap(QPixmap());
        setSceneRect(QRectF(0,0,m_baseFogOfWar.width(),m_baseFogOfWar.width()));
    }
    updateGrid();
    updateFogOfWar();
    startTimer(30);
}
#include <algorithm>
#include <cmath>

// Helper struct for sorting
struct RayPoint {
    QPointF point;
    double angle;
    bool operator<(const RayPoint& other) const {
        return angle < other.angle;
    }
};

QPolygonF calculateVisibilityPolygon(QPointF lightCenter, double radius, const QVector<QLineF>& walls) {
    QVector<RayPoint> rayPoints;

    // 1. Collect all unique angles we need to cast rays towards
    // We need the endpoints of every wall
    for (const QLineF& wall : walls) {
        QPointF p1 = wall.p1();
        QPointF p2 = wall.p2();

        // Add rays for p1 and p2
        // NOTE: Ideally, you add +/- tiny epsilon offsets to hit JUST behind the wall
        double angle1 = std::atan2(p1.y() - lightCenter.y(), p1.x() - lightCenter.x());
        double angle2 = std::atan2(p2.y() - lightCenter.y(), p2.x() - lightCenter.x());

        rayPoints.append({p1, angle1});
        rayPoints.append({p2, angle2});

        // Add the +/- epsilon rays here (omitted for brevity, but crucial for quality)
        // These help you see "past" a corner
    }

    // Also add corners of the screen or bounding box if necessary so the light has a 'container'

    // 2. Sort rays by angle
    std::sort(rayPoints.begin(), rayPoints.end());

    QPolygonF visibilityPoly;

    // 3. Cast rays
    for (const auto& rp : rayPoints) {
        QLineF ray(lightCenter, lightCenter + QPointF(std::cos(rp.angle), std::sin(rp.angle)) * radius * 2); // Long ray

        QPointF closestHit;
        double closestDist = std::numeric_limits<double>::max();

        // Check intersection against ALL walls
        for (const QLineF& wall : walls) {
            QPointF intersection;
            // QLineF::BoundedIntersection checks strictly within the wall segment
            if (wall.intersects(ray, &intersection) == QLineF::BoundedIntersection) {
                double dist = QLineF(lightCenter, intersection).length();
                if (dist < closestDist) {
                    closestDist = dist;
                    closestHit = intersection;
                }
            }
        }

        if (closestDist < std::numeric_limits<double>::max()) {
            visibilityPoly << closestHit;
        } else {
             // If we hit nothing, the light goes to the max radius
             visibilityPoly << ray.p2();
        }
    }

    return visibilityPoly;
}
void Scene::updateFogOfWar()
{
    QPainter painter;
    QPixmap pixmap = m_baseFogOfWar;
    if (pixmap.isNull()) return;

    painter.begin(&pixmap);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    // 1. Get all walls from your scene (pseudo-code)
    QVector<QLineF> allWalls;
    allWalls.reserve(m_walls.size());
    for(WallItem* item:m_walls)
    {
        allWalls.append(item->walls() );
    }

    for (int i = 0; i < m_mapItems.size(); i++)
    {
        MapItem* item = m_mapItems[i];
        QPixmap light = item->createLight();
        if (light.isNull()||item->lightRadius()==0) continue;

        // 1. Calculate the visibility polygon for THIS light
        // (Optimization: Filter 'allWalls' to only those close to this light first)
        QPointF lightCenter(item->x(), item->y());
        double radius = std::max(light.width(), light.height()); // Approximate radius
        QTransform transform;
        transform.translate(item->x(), item->y());
        transform.rotate(item->lightRotation());
        QVector<QLineF> nearbyWalls;
        QPolygonF lightBounds=transform.map(QPolygonF(QRectF(-light.width() / 2, -light.height() / 2,light.width(),light.height())));

        for(auto& wall : allWalls) {
            // Quick check if wall is somewhat inside light radius
            if (lightBounds.intersects(QRectF(wall.p1(), wall.p2()).normalized())) {
                nearbyWalls.append(wall);
            }
        }
        double r = radius;
        QPointF c = lightCenter;
        // Create a box centered on the light
        QVector<QLineF> boundaryWalls;
        if(lightBounds.size()==5)
        {
            boundaryWalls << QLineF(lightBounds.at(0),lightBounds.at(1)); // Top
            boundaryWalls << QLineF(lightBounds.at(1),lightBounds.at(2)); // Right
            boundaryWalls << QLineF(lightBounds.at(2),lightBounds.at(3)); // Bottom
            boundaryWalls << QLineF(lightBounds.at(3),lightBounds.at(0)); // Left

        }
        // 2. Combine your real map walls with these boundary walls
        QVector<QLineF> checkingWalls = nearbyWalls + boundaryWalls;


        QPolygonF visPoly = calculateVisibilityPolygon(lightCenter, radius, checkingWalls);

        painter.save();

        // 2. Apply the clipping path!
        // This is the magic. The painter will ONLY draw where the polygon exists.
        QPainterPath path;
        path.addPolygon(visPoly);
        painter.setClipPath(path);

        // 3. Setup the composition mode to erase fog
        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);

        // 4. Draw the light exactly as you did before
        painter.setTransform(transform);
        painter.drawPixmap(-light.width() / 2, -light.height() / 2, light);

        painter.restore();
        // Clipping is removed when we restore()
    }
    painter.end();
    m_fogOfWar->setPixmap(pixmap);
}
/*
void Scene::updateFogOfWar()
{

    QPainter painter;
    QPixmap pixmap=m_baseFogOfWar;
    if(pixmap.isNull())return;
    painter.begin(&pixmap);
    painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    for(int i=0;i<m_mapItems.size();i++)
    {
        MapItem* item= m_mapItems[i];
        if(item->lightRadius()<=0||item->blockLight())
            continue;
        QPixmap light=item->createLight();        
        if(light.isNull())
            continue;
        for(int j=0;j<m_mapItems.size();j++)
        {
            MapItem* negItem= m_mapItems[j];
            if(negItem->lightRadius()<=0||!negItem->blockLight())
                continue;
            QPixmap negLight=item->createLight();
            if(negLight.isNull())
                continue;
            QPainter negPainter;
            negPainter.begin(&light);
            negPainter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing);
            negPainter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
            negPainter.translate(negItem->x()-item->x(),negItem->y()-item->y());
            negPainter.rotate(negItem->lightRotation());
            negPainter.drawPixmap(-negLight.width()/2,-negLight.height()/2,negLight);

        }
        painter.save();
        painter.translate(item->x(),item->y());
        painter.rotate(item->lightRotation());
        painter.drawPixmap(-light.width()/2,-light.height()/2,light);
        painter.restore();
    }
    painter.end();
    m_fogOfWar->setPixmap(pixmap);

}
*/
void Scene::updateGrid()
{

    QPainter painter;
    QPixmap pixmap=m_grid->pixmap();
    if(pixmap.isNull())return;
    pixmap.fill(Qt::transparent);
    painter.begin(&pixmap);
    painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing);
    drawGrid(&painter,QRectF(QPointF(),pixmap.size()));
    painter.end();
    m_grid->setPixmap(pixmap);

}
void Scene::hideScene()
{
    m_curser->hide();
    m_hide=true;
    invalidate();
}

void Scene::setLinearStart(const QPointF &position)
{
    if(m_background->pixmap().isNull())
        return;
    m_linear->setStartPoint(position);
    m_linear->show();
    invalidate(m_linear->boundingRect());
}

void Scene::hideLinear()
{
    m_linear->hide();
}

void Scene::setLinearEnd(const QPointF &position)
{
    if(m_background->pixmap().isNull())
        return;
    m_linear->setEndPoint(position);
    invalidate(m_linear->boundingRect());
}

void Scene::setCursorPos(const QPointF &position)
{
    if(m_background->pixmap().isNull())
        return;
    m_curser->setPos(position);
    if(!sceneRect().contains(position))
    {
        m_curser->hide();
    }
    else
    {
        m_curser->show();
    }
    invalidate(m_curser->boundingRect());
}

void Scene::updateScene()
{
    m_dirty=true;
}

void Scene::updateNow()
{

    for(int i=0;i<m_mapItems.size();i++)
    {
        delete m_mapItems[i];
    }
    m_mapItems.clear();
    for(int i=0;i<m_walls.size();i++)
    {
        delete m_walls[i];
    }
    m_walls.clear();
    m_viewportItem->setRect(m_otherScene->m_viewportItem->x(),
                            m_otherScene->m_viewportItem->y(),
                            m_otherScene->m_viewportItem->width(),
                            m_otherScene->m_viewportItem->height());
    if(!m_otherScene)
    {
        updateFogOfWar();
        return;
    }
    if(m_gridSize!=m_otherScene->m_gridSize||
            m_gridXOffset!=m_otherScene->m_gridXOffset||
            m_gridYOffset!=m_otherScene->m_gridYOffset )
        updateGrid();
    m_gridSize=m_otherScene->m_gridSize;
    m_gridXOffset=m_otherScene->m_gridXOffset;
    m_gridYOffset=m_otherScene->m_gridYOffset;
    m_fogOfWar->setVisible(m_otherScene->showFogOfWar());
    for(int i=0;i<m_otherScene->m_mapItems.size();i++)
    {
        addMarker(m_otherScene->m_mapItems[i]->createPresentItem());
    }
    for(int i=0;i<m_otherScene->m_walls.size();i++)
    {
        WallItem* item=new WallItem(*m_otherScene->m_walls[i]);
        m_walls.append(item);
        //addItem(item);
    }
    updateFogOfWar();
}

int Scene::gridXOffset() const
{
    return m_gridXOffset;
}

void Scene::setGridXOffset(int newGridXOffset)
{
    m_gridXOffset = newGridXOffset;
    updateGrid();
}

int Scene::gridYOffset() const
{
    return m_gridYOffset;
}

void Scene::setGridYOffset(int newGridYOffset)
{
    m_gridYOffset = newGridYOffset;
    updateGrid();
}

bool Scene::showFogOfWar() const
{
    return m_showFogOfWar;
}

void Scene::setShowFogOfWar(bool newShowFogOfWar)
{
    m_showFogOfWar = newShowFogOfWar;
    m_fogOfWar->setVisible(m_showFogOfWar);
}

void Scene::removeMarker(MapItem *marker)
{

    m_mapItems.removeAll(marker);
    removeItem(marker);
    updateFogOfWar();
    invalidate();

}

int Scene::gridSize() const
{
    return m_gridSize;
}

void Scene::setGridSize(int newGridSize)
{
    m_gridSize = newGridSize;
    updateGrid();
}

void Scene::syncScene(Scene *other)
{

    m_curser->hide();
    m_hide=false;
    if(m_otherScene)
        disconnect(m_otherScene,&Scene::changed,this,&Scene::updateScene);
    m_otherScene=other;
    if(!other)
    {
        updateNow();
        return;
    }
    connect(other,&Scene::changed,this,&Scene::updateScene);


    QPixmap background=QPixmap(other->m_path).scaled(QSize(1920,1080),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    m_background->setPixmap(background);
    QPainter painter;
    m_baseFogOfWar=QPixmap(m_background->pixmap().size());
    m_grid->setPixmap(QPixmap(m_background->pixmap().size()));
    m_baseFogOfWar.fill( Qt::transparent);
    QRect rect(0,0,m_baseFogOfWar.width(),m_baseFogOfWar.height());
    painter.begin(&m_baseFogOfWar);
    painter.fillRect(rect,m_fogOfWarBrush);

    setSceneRect(rect);
    painter.end();
    updateGrid();

    updateNow();
}

void Scene::drawGrid(QPainter *painter, const QRectF &rect)
{
    if(m_hide||m_gridSize<=2)
    {
       // painter->fillRect(rect,Qt::black);
    }
    else
    {
        painter->setPen(QPen(QColor(40,40,40,m_gridOpaque*255),m_gridThickness));
        for(int i=0;i<rect.width()/m_gridSize;i++)
        {
            painter->drawLine((int)rect.x()-((int)rect.x()%m_gridSize)+i*m_gridSize+m_gridXOffset,rect.y()+m_gridYOffset,
                              (int)rect.x()-((int)rect.x()%m_gridSize)+i*m_gridSize+m_gridXOffset,rect.bottom()+m_gridYOffset);
        }
        for(int i=0;i<rect.height()/m_gridSize;i++)
        {
            painter->drawLine(rect.x()+m_gridXOffset,(int)rect.y()-((int)rect.y()%m_gridSize)+i*m_gridSize+m_gridYOffset,
                              rect.right()+m_gridXOffset,(int)rect.y()-((int)rect.y()%m_gridSize)+i*m_gridSize+m_gridYOffset);
        }
    }

}

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{

    painter->fillRect(rect,m_fogOfWarBrush);
}

void Scene::timerEvent(QTimerEvent *)
{
    if(!m_dirty)
        return;
    m_dirty=false;
    updateNow();
}

QList<WallItem *> Scene::walls() const
{
    return m_walls;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch(m_drawWallsMode)
    {
    case NoDraw:
        break;
    case DrawingPlacing:
        if(m_floatingWalls)
        {
            m_floatingWalls->setPos(event->scenePos());
            m_floatingWalls->addEndPos(event->scenePos());
            m_drawWallsMode= DrawingMoving;
        }
        return;
        break;
    case DrawingMoving:
         if(m_floatingWalls)
        {
             if(event->modifiers()&(Qt::ControlModifier|Qt::AltModifier|Qt::ShiftModifier))
             {
                 m_floatingWalls->addEndPos(event->scenePos());
             }
             else
             {
                 WallItem* item=new WallItem(*m_floatingWalls);

                 m_walls.append(item);
                 addItem(item);
                 delete m_floatingWalls;
                 m_floatingWalls=0;
                 m_drawWallsMode= DrawingPlacing;
             }
            return;
        }

        break;
    }
    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    switch(m_drawWallsMode)
    {
    case NoDraw:
        break;
    case DrawingPlacing:
        if(m_floatingWalls==0)
        {
            m_floatingWalls=new WallItem;
            addItem(m_floatingWalls);
        }
        m_floatingWalls->setPos(event->scenePos());
        return;
        break;
    case DrawingMoving:
        if(m_floatingWalls)
        {
            m_floatingWalls->setEndPos(event->scenePos());
            return;
        }

        break;
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

bool Scene::drawWallsMode() const
{
    return m_drawWallsMode;
}

void Scene::setDrawWallsMode(bool newDrawWallsMode)
{
    if(!newDrawWallsMode)
    {
        m_drawWallsMode = NoDraw;
        delete m_floatingWalls;
        m_floatingWalls=nullptr;
    }
    else
    {
        m_drawWallsMode = DrawingPlacing;
    }
}

void Scene::deleteWall(WallItem *item)
{
    for(int i=0;i<m_walls.size();i++)
    {
        if(m_walls[i]==item)
        {
            delete m_walls.takeAt(i);
            return;
        }
    }
    updateFogOfWar();
}

ViewportItem *Scene::viewportItem() const
{
    return m_viewportItem;
}

QList<MapItem *> Scene::mapItems() const
{
    return m_mapItems;
}

QGraphicsPixmapItem *Scene::background() const
{
    return m_background;
}

void Scene::addWall(double x1,double y1,const QList<QPointF>& points)
{
    WallItem* item=new WallItem(x1,y1,points);
    m_walls.append(item);
    addItem(item);
}

void Scene::addMarker(MapItem *item)
{
    m_mapItems<<item;
    item->setZValue(m_mapItems.size()+1);
    addItem(item);
    item->reload();
}
