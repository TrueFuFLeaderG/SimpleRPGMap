#ifndef SCENE_H
#define SCENE_H

#include <Linear.h>
#include <MapItem.h>
#include <QGraphicsScene>
#include <QObject>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(const QString& path="");
    void updateFogOfWar();
    void hideScene();
    void setCursorPos(const QPointF& position);
    void syncScene(Scene* other);
    QGraphicsPixmapItem *background() const;
    void addMarker(MapItem* item);
    int gridSize() const;
    void setGridSize(int newGridSize);

    bool showFogOfWar() const;
    void setShowFogOfWar(bool newShowFogOfWar);
    void removeMarker(MapItem* marker);

    int gridYOffset() const;
    void setGridYOffset(int newGridYOffset);

    int gridXOffset() const;
    void setGridXOffset(int newGridXOffset);

    QList<MapItem *> mapItems() const;

    void setLinearEnd(const QPointF &position);
    void setLinearStart(const QPointF &position);
    void updateGrid();
public slots:
    void updateScene();
private:
    void updateNow();
    bool m_dirty=false;
    bool m_showFogOfWar=true;
    int m_gridSize=32;
    int m_gridXOffset=0;
    int m_gridYOffset=0;
    float m_fogOfWarOpaque=0.5;
    float m_gridOpaque=0.8;
    float m_gridThickness=1;
    // QGraphicsScene interface
protected:
    void drawGrid(QPainter *painter, const QRectF &rect);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void timerEvent(QTimerEvent *event);
private:

    Linear* m_linear=new Linear();
    QBrush m_fogOfWarBrush;
    QString m_path;
    QGraphicsPixmapItem* m_fogOfWar;
    QGraphicsEllipseItem* m_curser;
    QGraphicsPixmapItem* m_background;
    QGraphicsPixmapItem* m_grid;
    QList<MapItem*> m_mapItems;
    QPixmap m_baseFogOfWar;
    bool m_hide=false;
    Scene* m_otherScene=0;


};

#endif // SCENE_H
