#include "mapcontrol.h"

#include <MapItem.h>
#include <QDragEnterEvent>
#include <QMimeData>
#include <Scene.h>

MapControl::MapControl(const QString &path)
{
    setScene(m_scene=new Scene(path));
}

MapControl::MapControl(bool present):m_present(present)
{
    if(present)
    {
        setDragMode(ScrollHandDrag);
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);
        setResizeAnchor(QGraphicsView::AnchorViewCenter);
    }
    setScene(m_scene=new Scene);
}

void MapControl::hideScene()
{
    m_scene->hideScene();
}

void MapControl::syncScene(MapControl *other)
{
    if(other)
        m_scene->syncScene(other->scene());
    else
        m_scene->syncScene(0);
}

void MapControl::dragEnterEvent(QDragEnterEvent *event)
{
    QGraphicsView::dragEnterEvent(event);
    const QMimeData* data=event->mimeData();
    if(data->hasFormat("application/x-qabstractitemmodeldatalist"))
        event->accept();
}

void MapControl::dragMoveEvent(QDragMoveEvent *event)
{
    QGraphicsView::dragMoveEvent(event);
    const QMimeData* data=event->mimeData();
    if(data->hasFormat("application/x-qabstractitemmodeldatalist"))
        event->accept();
}

void MapControl::dragLeaveEvent(QDragLeaveEvent *event)
{
    QGraphicsView::dragLeaveEvent(event);
}

void MapControl::dropEvent(QDropEvent *event)
{
    QGraphicsView::dropEvent(event);
    const QMimeData* data=event->mimeData();
    QByteArray encoded = data->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;
        QString path=roleDataMap[Qt::UserRole].toString();
        MapItem* item=new MapItem(path);
        item->setPos(mapToScene(event->position().toPoint()));
        item->show();
        scene()->addMarker(item);
        scene()->updateFogOfWar();

        break;
    }
}

void MapControl::wheelEvent(QWheelEvent *event)
{
    double num=event->angleDelta().y()/360.0/10;
    scale(1-num,1-num);


}

void MapControl::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyReleaseEvent(event);
    if(event->text()=="0")
    {
        resetTransform();
    }
}


Scene *MapControl::scene() const
{
    return m_scene;
}
