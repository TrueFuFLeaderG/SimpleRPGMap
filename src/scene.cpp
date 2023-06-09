#include "scene.h"

#include <MapItem.h>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <qgraphicsview.h>

Scene::Scene(const QString &path):m_path(path)
{
    m_fogOfWarBrush=QBrush(QPixmap("./fogofwar.png"));
    m_curser=new QGraphicsEllipseItem(-8,-8,16,16);
    addItem(m_linear);
    m_curser->setBrush(QColor(255,0,0,100));
    addItem(m_curser);
    m_curser->setZValue(10000);
    m_linear->setZValue(10000);
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
    }
    else
    {
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

void Scene::updateFogOfWar()
{

    QPainter painter;
    QPixmap pixmap=m_baseFogOfWar;
    if(pixmap.isNull())return;
    painter.begin(&pixmap);
    painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing);
    for(int i=0;i<m_mapItems.size();i++)
    {
        MapItem* item= m_mapItems[i];
        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
        QPixmap light=item->createLight();
        if(light.isNull())
            continue;
        painter.save();
        painter.translate(item->x(),item->y());
        painter.rotate(item->lightRotation());
        painter.drawPixmap(-light.width()/2,-light.height()/2,light);
        painter.restore();
    }
    painter.end();
    m_fogOfWar->setPixmap(pixmap);

}

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
    if(!sceneRect().contains(position))
    {
        m_linear->hide();
        return;
    }
    m_linear->setStartPoint(position);
    m_linear->show();
    if(!sceneRect().contains(position))
    {
        m_linear->hide();
    }
    else
    {
        m_linear->show();
    }
    invalidate(m_linear->boundingRect());
}

void Scene::setLinearEnd(const QPointF &position)
{
    if(m_background->pixmap().isNull())
        return;
    m_linear->setEndPoint(position);
    if(!sceneRect().contains(position))
    {
        m_linear->hide();
    }
    else
    {
        m_linear->show();
    }
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
        removeItem(m_mapItems[i]);
    }
    m_mapItems.clear();
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

void Scene::timerEvent(QTimerEvent *event)
{
    if(!m_dirty)
        return;
    m_dirty=false;
    updateNow();
}

QList<MapItem *> Scene::mapItems() const
{
    return m_mapItems;
}

QGraphicsPixmapItem *Scene::background() const
{
    return m_background;
}

void Scene::addMarker(MapItem *item)
{
    m_mapItems<<item;
    item->setZValue(m_mapItems.size()+1);
    addItem(item);
}
