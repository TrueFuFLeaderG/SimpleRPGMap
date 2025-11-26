#include "wallitem.h"
#include "QPen"

#include <Scene.h>
WallItem::WallItem()
{
    setDrawData();
}
WallItem::WallItem(double x1, double y1,const QList<QPointF>& points)
{
    setDrawData();

    setPos(x1,y1);
    for(QPointF point:points)
        m_endPos.append(new WallEnd(this,point));

    updateLine();
}
QList<QPointF> WallItem::rawPoints() const
{
    QList<QPointF> ret;
    for(WallEnd* wall:m_endPos)
    {
        ret<<wall->pos();
    }
    return ret;
}

QList<QLineF> WallItem::walls() const
{
    QList<QLineF> ret;
    QPointF p=pos();
    for(WallEnd* wall:m_endPos)
    {
        ret<<QLineF(p,pos()+wall->pos());
        p=pos()+wall->pos();
    }
    return ret;
}

void WallItem::setDrawData()
{

    QPen pen((QColor(Qt::cyan)));
    pen.setWidthF(3);
    setPen(pen);
    setRect(-10,-10,20,20);
    setFlag(ItemIsMovable,false);
    setFlag(ItemIsSelectable,false);
    setFlag(ItemIsFocusable,false);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges,true);

    setFlag(ItemIsMovable,true);
    setFlag(ItemIsSelectable,true);
    setFlag(ItemIsFocusable,true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges,true);
}

void WallItem::updateLine(WallEnd *point)
{
    while(m_lines.size()<m_endPos.size())
    {
        QGraphicsLineItem* item=new  QGraphicsLineItem(this);
        item->setPen(QPen(QColor(Qt::cyan),3));
        m_lines.append(item);
    }
    while(m_lines.size()>m_endPos.size())
    {
        delete m_lines.takeFirst();
    }
    for(int i=0;i<m_endPos.size();i++)
    {
        QPointF lastPos(0,0);
        if(i)
            lastPos=m_endPos[i-1]->pos();
        if(!point||m_endPos[i]==point)
        {
            m_lines[i]->setLine(lastPos.x(),lastPos.y(),m_endPos[i]->pos().x(),m_endPos[i]->pos().y());
            if(i)
                m_lines[i]->setLine(m_endPos[i-1]->pos().x(),m_endPos[i-1]->pos().y(),m_endPos[i]->pos().x(),m_endPos[i]->pos().y());
        }
    }
    Scene* fogscene = qobject_cast<Scene*>(scene());
    if(fogscene)
        fogscene->updateFogOfWar();
}

QVariant WallItem::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if(change==ItemPositionChange)
        updateLine();
    return value;
}



void WallItem::setEndPos(const QPointF &p)
{
    if(m_endPos.isEmpty())
        return;
       m_endPos.last()->setPos(p-pos());
      updateLine(m_endPos.last());
}
void WallItem::addEndPos(const QPointF &p)
{

    m_endPos.append(new WallEnd(this,p-pos()));
    updateLine();
}

void WallItem::wallChanged(WallEnd *point)
{
    updateLine(point);
}
WallItem::WallItem(const WallItem& other)
{
    setDrawData();
    setPos(other.pos());
    for(WallEnd* wall:other.m_endPos)
        m_endPos.append(new WallEnd(this,wall->pos()));
    updateLine();
}



QVariant WallEnd::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change==ItemPositionChange)
    {
        if(parentItem()&&parentItem()->type() == WallItem::Type)
        {
            ((WallItem*)parentItem())->wallChanged(this);
        }
    }
    return value;
}

WallEnd::WallEnd(WallItem *parent, const QPointF &p):
    QGraphicsEllipseItem(parent)
{
    QPen pen((QColor(Qt::cyan)));
    pen.setWidthF(3);

    setPen(pen);
    setRect(-10,-10,20,20);
    setRect(-8,-8,16,16);
    setFlag(ItemIsMovable,true);
    setFlag(ItemIsSelectable,true);
    setFlag(ItemIsFocusable,true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges,true);

    setPos(p);
}

WallItem *WallEnd::wall() const
{
    if(parentItem()&&parentItem()->type() == WallItem::Type)
    {
       return ((WallItem*)parentItem());
    }
    return 0;
}

