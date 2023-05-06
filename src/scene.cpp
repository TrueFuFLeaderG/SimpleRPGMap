#include "scene.h"

#include <MapItem.h>
#include <QGraphicsPixmapItem>
#include <QPainter>

Scene::Scene(const QString &path):m_path(path)
{
    m_fogOfWarBrush=QBrush(QPixmap("./fogofwar.png"));
    if(path!="")
    {
        QPixmap pixmap=QPixmap(path);
        m_background=addPixmap(pixmap);

        QPainter painter;
        m_baseFogOfWar=QPixmap(pixmap.size());
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
        m_fogOfWar=new QGraphicsPixmapItem(m_baseFogOfWar);
        m_fogOfWar->setZValue(1000);
        addItem(m_fogOfWar);
        m_background=addPixmap(QPixmap());
    }
    updateFogOfWar();
}

void Scene::updateFogOfWar()
{

    QPainter painter;
    QPixmap pixmap=m_baseFogOfWar;

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

void Scene::hideScene()
{
    m_hide=true;
    invalidate();
}

void Scene::updateScene()
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
    m_gridSize=m_otherScene->m_gridSize;
    m_fogOfWar->setVisible(m_otherScene->showFogOfWar());
    for(int i=0;i<m_otherScene->m_mapItems.size();i++)
    {
        addMarker(m_otherScene->m_mapItems[i]->createPresentItem());
    }
    updateFogOfWar();
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
    invalidate();
}

void Scene::syncScene(Scene *other)
{

    m_hide=false;
    if(m_otherScene)
        disconnect(m_otherScene,&Scene::changed,this,&Scene::updateScene);
    m_otherScene=other;
    if(!other)
    {
        updateScene();
        return;
    }
    connect(other,&Scene::changed,this,&Scene::updateScene);

    m_background->setPixmap(QPixmap(other->m_path));

    QPainter painter;
    m_baseFogOfWar=QPixmap(m_background->pixmap().size());
    m_baseFogOfWar.fill( Qt::transparent);

    painter.begin(&m_baseFogOfWar);
    painter.fillRect(0,0,m_baseFogOfWar.width(),m_baseFogOfWar.height(),m_fogOfWarBrush);
    painter.end();

    updateScene();
}

void Scene::drawForeground(QPainter *painter, const QRectF &rect)
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
            painter->drawLine((int)rect.x()-((int)rect.x()%m_gridSize)+i*m_gridSize,rect.y(),
                              (int)rect.x()-((int)rect.x()%m_gridSize)+i*m_gridSize,rect.bottom());
        }
        for(int i=0;i<rect.height()/m_gridSize;i++)
        {
            painter->drawLine(rect.x(),(int)rect.y()-((int)rect.y()%m_gridSize)+i*m_gridSize,
                              rect.right(),(int)rect.y()-((int)rect.y()%m_gridSize)+i*m_gridSize);
        }
    }

}

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{

    painter->fillRect(rect,m_fogOfWarBrush);
}

QGraphicsPixmapItem *Scene::background() const
{
    return m_background;
}

void Scene::addMarker(MapItem *item)
{
    m_mapItems<<item;
    addItem(item);
}
