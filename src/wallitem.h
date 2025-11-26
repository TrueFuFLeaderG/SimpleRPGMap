#ifndef WALLITEM_H
#define WALLITEM_H

#include <QGraphicsItemGroup>

class WallItem;
class WallEnd : public QGraphicsEllipseItem
{
public:
    enum
    {
      Type=3132
    };
    WallEnd(WallItem* parent,const QPointF& p=QPointF());
    int type() const{return Type;}
    WallItem *wall() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};
class WallItem : public QGraphicsRectItem
{
public:
    enum
    {
      Type=3131
    };

    WallItem();
    WallItem(const WallItem &other);
    WallItem(double x1, double y1, const QList<QPointF> &points);

    void setEndPos(const QPointF& p);
    void addEndPos(const QPointF& p);
    void wallChanged(WallEnd* point);
    void setDrawData();
    int type() const{return Type;}
    QList<QLineF> walls()const;
    QList<QPointF> rawPoints()const;
private:
    QList<QGraphicsLineItem*> m_lines;
    QList<WallEnd*> m_endPos;
    void updateLine(WallEnd *point=0);



    // QGraphicsItem interface
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

};

#endif // WALLITEM_H
