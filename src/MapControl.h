#ifndef MAPCONTROL_H
#define MAPCONTROL_H

#include <QGraphicsView>


class Scene;
class MapControl : public QGraphicsView
{
    Q_OBJECT
public:
    MapControl(const QString& path,bool present=false);

    void hideScene();
    void syncScene(MapControl* other);
    // QWidget interface
    Scene *scene() const;

    QString path() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    QString m_path;
    Scene* m_scene;
    bool m_present=false;


    // QWidget interface
protected:
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // MAPCONTROL_H
