#ifndef LINEAR_H
#define LINEAR_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>

class Linear : public QGraphicsItemGroup
{
public:
    Linear();

    void setStartPoint(const QPointF& p);
    void setEndPoint(const QPointF& p);
    void updateText();
private:

    QGraphicsLineItem* m_line=new QGraphicsLineItem;
    QGraphicsTextItem* m_text=new QGraphicsTextItem;
    QGraphicsEllipseItem* m_circle=new QGraphicsEllipseItem;
};

#endif // LINEAR_H
