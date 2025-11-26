#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QPixmap>

class ScaleItem;
class MapItem : public QGraphicsItem
{
public:
    MapItem(const QString& path);
    MapItem(const QJsonObject& obj);
    void loadFromFile(const QString& path);
    void loadFromObj(const QJsonObject& obj);
    MapItem* createPresentItem();
    void reload();
    QPixmap createLight();
    QPixmap createImage();
    QPixmap icon();
    QString name() const;
    void setName(const QString &newName);

    enum {
        Type = UserType
    };
    int type() const{return Type;}
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QJsonObject toObj();
    bool showName() const;
    void setShowName(bool newShowName);

    double labelSize() const;
    void setLabelSize(double newLabelSize);

    QString img() const;
    void setImg(const QString &newImg);

    double radius() const;
    void setRadius(double newRadius);

    double aspectratio() const;
    void setAspectratio(double newAspectratio);

    double rotation() const;
    void setRotation(double newRotation);

    QString light() const;
    void setLight(const QString &newLight);

    double lightRadius() const;
    void setLightRadius(double newLightRadius);

    double lightAspectratio() const;
    void setLightAspectratio(double newLightAspectratio);

    double lightRotation() const;
    void setLightRotation(double newLightRotation);

    void remove();
    QString color() const;
    void setColor(const QString &newColor);

    bool removeBackground() const;
    void setRemoveBackground(bool newRemoveBackground);



    QString notes() const;
    void setNotes(const QString &newNotes);

    bool blockLight() const;
    void setBlockLight(bool newBlockLight);

    bool present() const;

    bool scaleable();
signals:
    void nameChanged();

private:
    QString m_path;
    QString m_name= "Player";
    QString m_color= "";
    bool m_showName=true;
    bool m_removeBackground=true;
    double m_labelSize=16;
    QString m_img="marker2.png";
    double m_radius=8;
    double m_aspectratio=1;
    double m_rotation=0;
    QString m_light="circleSmoother.png";
    double m_lightRadius=16;
    double m_lightAspectratio=1;
    double m_lightRotation=0;
    QString m_notes;
    bool m_blockLight=true;
    bool m_present=false;

    QGraphicsPixmapItem* m_pixmapItem;
    QGraphicsTextItem* m_textItem;
    ScaleItem* m_scalItem=0;


protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

};

class ScaleItem : public QGraphicsItem
{
public:
    ScaleItem(MapItem* item);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void updatePos();


private:
    bool m_state=false;
    double m_penWidth=2;

    QRectF m_scaleRect=QRectF(0,0,24,24);
    MapItem* m_item;
    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};
#endif // MAPITEM_H
