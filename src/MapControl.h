#ifndef MAPCONTROL_H
#define MAPCONTROL_H

#include <QGraphicsView>


class Scene;
class MapControl : public QGraphicsView
{
    Q_OBJECT
public:
    MapControl(const QString& path);
    MapControl(bool present);

    void hideScene();
    void syncScene(MapControl* other);
    // QWidget interface
    Scene *scene() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    Scene* m_scene;
    bool m_present=false;


    // QWidget interface
protected:
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAPCONTROL_H
