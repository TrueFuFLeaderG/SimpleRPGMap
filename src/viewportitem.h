#ifndef VIEWPORTITEM_H
#define VIEWPORTITEM_H

#include <QGraphicsItemGroup>

class ViewportItem : public QGraphicsItem
{
public:
    ViewportItem();
    void setRect(double x,double y,double w, double h);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    enum ActionState
    {
        None,
        Move,
        Scale
    };

    double height() const;

    double width() const;

private:
    ActionState m_state=None;
    double m_height=0;
    double m_width=0;
    double m_relation=16./9. ;
    double m_penWidth=2;
    QRectF m_moveRect=QRectF(m_penWidth+2,m_penWidth+2,24,24);
    QRectF m_scaleRect=QRectF(m_penWidth+2+24+4,m_penWidth+2,24,24);

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // VIEWPORTITEM_H
