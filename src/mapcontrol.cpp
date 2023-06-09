#include "mapcontrol.h"

#include <MainWindow.h>
#include <MapItem.h>
#include <QDragEnterEvent>
#include <QMimeData>
#include <Scene.h>

MapControl::MapControl(const QString &path, bool present):m_path(path),m_present(present)
{
    if(present)
    {
        setDragMode(ScrollHandDrag);
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);
        setResizeAnchor(QGraphicsView::AnchorViewCenter);
    }
    else
    {
        setMouseTracking(true);
    }
    setScene(m_scene=new Scene(path));
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

    resetTransform();
    double ratioW=size().width()/(double)m_scene->background()->pixmap().width();
    double ratioH=size().height()/(double)m_scene->background()->pixmap().height();
    scale(qMin(ratioH,ratioW),qMin(ratioH,ratioW));

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

QString MapControl::path() const
{
    return m_path;
}

void MapControl::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsView::keyReleaseEvent(event);
    if(event->text()=="0")
    {
        resetTransform();
    }
    if(event->key()==Qt::Key_Delete||event->key()==Qt::Key_Backspace)
    {
        MainWindow::mainWindow()->markerProperties()->deleteCurrentItem();
    }
}

void MapControl::mouseMoveEvent(QMouseEvent *event)
{


    QGraphicsView::mouseMoveEvent(event);
    MapControl* map=MainWindow::mainWindow()->presetMap();
    if(map)
        map->scene()->setCursorPos( mapToScene(event->pos()));
}

void MapControl::leaveEvent(QEvent *event)
{

    QGraphicsView::leaveEvent(event);
    MapControl* map=MainWindow::mainWindow()->presetMap();
    if(map)
        map->scene()->setCursorPos( QPointF(-1,-1));
}

void MapControl::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    resetTransform();
    double ratioW=size().width()/(double)m_scene->background()->pixmap().width();
    double ratioH=size().height()/(double)m_scene->background()->pixmap().height();
    scale(qMin(ratioH,ratioW),qMin(ratioH,ratioW));

}


Scene *MapControl::scene() const
{
    return m_scene;
}
