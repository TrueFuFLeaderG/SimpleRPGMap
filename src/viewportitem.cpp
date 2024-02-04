#include "viewportitem.h"

#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <qpainter.h>
#include <QDebug>
#include <QtMath>
#include <QPainterPath>
#include <QIcon>

ViewportItem::ViewportItem()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void ViewportItem::setRect(double x, double y, double w, double h)
{
    setPos(x,y);
    m_width=w;
    m_height=h;

}

QRectF ViewportItem::boundingRect() const
{
    return QRectF(-1,-1,m_width+m_penWidth*2,m_height+m_penWidth*2);
}
void drawArrowLeft(QPainter *painter,double x,double y)
{

    painter->drawLine(x,y,x+2,y-2);
    painter->drawLine(x,y,x+2,y+2);
}
void drawArrowLeftUp(QPainter *painter,double x,double y)
{

    painter->drawLine(x,y,x+3,y);
    painter->drawLine(x,y,x,y+3);
}
void drawArrowDownRight(QPainter *painter,double x,double y)
{

    painter->drawLine(x,y,x-3,y);
    painter->drawLine(x,y,x,y-3);
}
void drawArrowRight(QPainter *painter,double x,double y)
{

    painter->drawLine(x,y,x-2,y-2);
    painter->drawLine(x,y,x-2,y+2);
}
void drawArrowUp(QPainter *painter,double x,double y)
{

    painter->drawLine(x,y,x+2,y+2);
    painter->drawLine(x,y,x-2,y+2);
}
void drawArrowDown(QPainter *painter,double x,double y)
{

    painter->drawLine(x,y,x+2,y-2);
    painter->drawLine(x,y,x-2,y-2);
}
void ViewportItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setPen(QPen(Qt::red,m_penWidth));
    painter->drawRect(painter->pen().widthF(),painter->pen().widthF(),m_width-painter->pen().widthF(),m_height-painter->pen().widthF());


    painter->setPen(QPen(Qt::black,m_penWidth));
    painter->setBrush(QBrush(Qt::red));
    if(m_state==Move)
        painter->setBrush(QBrush(Qt::red));
    else
        painter->setBrush(QBrush(Qt::gray));
    painter->drawEllipse(m_moveRect);


    painter->drawLine(m_moveRect.x()+m_penWidth*2+2,
                      m_moveRect.y()+m_moveRect.height()/2,
                      m_moveRect.x()+m_moveRect.width()-m_penWidth*2-2,
                      m_moveRect.y()+m_moveRect.height()/2);
    drawArrowLeft(painter,m_moveRect.x()+m_penWidth*2, m_moveRect.y()+m_moveRect.height()/2);
    drawArrowRight(painter,m_moveRect.x()+m_moveRect.width()-m_penWidth*2, m_moveRect.y()+m_moveRect.height()/2);

    painter->drawLine(m_moveRect.x()+m_moveRect.width()/2,
                      m_moveRect.y()+m_penWidth*2+2,
                      m_moveRect.x()+m_moveRect.width()/2,
                      m_moveRect.y()+m_moveRect.width()-m_penWidth*2-2);


    drawArrowUp(painter,m_moveRect.x()+m_moveRect.width()/2, m_moveRect.y()+m_penWidth*2);
    drawArrowDown(painter,m_moveRect.x()+m_moveRect.width()/2,m_moveRect.y()+m_moveRect.width()-m_penWidth*2);


    if(m_state==Scale)
        painter->setBrush(QBrush(Qt::red));
    else
        painter->setBrush(QBrush(Qt::gray));

    painter->drawEllipse(m_scaleRect);


    painter->drawLine(m_scaleRect.x()+(m_scaleRect.width()/4.)+1,
                      m_scaleRect.y()+(m_scaleRect.height()/4.)+1,
                      m_scaleRect.x()+(m_scaleRect.width()*(3./4.))-1,
                      m_scaleRect.y()+(m_scaleRect.height()*(3./4.))-1);
    drawArrowLeftUp(painter,m_scaleRect.x()+(m_scaleRect.width()/4.),
                  m_scaleRect.y()+(m_scaleRect.height()/4.));
    drawArrowDownRight(painter,
                   m_scaleRect.x()+(m_scaleRect.width()*(3./4.)),
                   m_scaleRect.y()+(m_scaleRect.height()*(3./4.)));

}

double ViewportItem::height() const
{
    return m_height;
}

double ViewportItem::width() const
{
    return m_width;
}

void ViewportItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_scaleRect.contains(event->pos()))
    {
        m_state=Scale;
        event->accept();
        update();
    }
    else if(m_moveRect.contains(event->pos()))
    {
        m_state=Move;
        event->accept();
        update();
    }
    else
    {
        m_state=None;
        update();
        QGraphicsItem::mousePressEvent(event);
    }
}

void ViewportItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QPointF delta=event->lastPos()-event->pos();
    switch(m_state)
    {
    case Scale:
        prepareGeometryChange();
        if(qAbs( delta.x())>qAbs(delta.y()))
        {
            m_width+=delta.x();
            if(m_width<100)
                m_width=100;
            m_height=m_width/m_relation;
        }
        else
        {
            m_height+=delta.y();
            if(m_height<100)
                m_height=100;
            m_width=m_height*m_relation;
        }
        moveBy(-delta.x(),-delta.y());
        break;
    case Move:
        moveBy(-delta.x(),-delta.y());
        break;
    default:;
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void ViewportItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_state=None;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void ViewportItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

    m_state=None;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void ViewportItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{

    m_state=None;
    QGraphicsItem::hoverLeaveEvent(event);
}
