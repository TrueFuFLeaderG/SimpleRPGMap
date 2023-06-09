#include "mapitem.h"

#include <QFile>
#include <QGraphicsColorizeEffect>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <Scene.h>
#include <qpainter.h>

MapItem::MapItem(const QString &path)
{

    m_pixmapItem=new QGraphicsPixmapItem(this);
    m_textItem= new QGraphicsTextItem(this);
    setVisible(true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges,true);
    loadFromFile(path);
}

MapItem::MapItem(const QJsonObject &path)
{

    m_pixmapItem=new QGraphicsPixmapItem(this);
    m_textItem= new QGraphicsTextItem(this);
    setVisible(true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges,true);
    loadFromObj(path);
}
void MapItem::loadFromFile(const QString &path)
{
    m_path=path;
    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        QJsonParseError err;
       QJsonDocument doc= QJsonDocument::fromJson(file.readAll(),&err);
       if(err.error!=QJsonParseError::NoError)
       {
           QMessageBox::warning(0,QObject::tr("Error parsing file %0").arg(path),QObject::tr("Error %0 offset %1").arg(err.errorString()).arg(err.offset));
            return;
       }
       loadFromObj(doc.object());
    }
    else
    {
        QMessageBox::warning(0,QObject::tr("can not open file %0").arg(path),QObject::tr("can not open file %0").arg(path));
        return;
    }

}

void MapItem::loadFromObj(const QJsonObject &obj)
{
    m_name=obj.value("name").toString("Player");
    m_showName=obj.value("showName").toBool(true);
    m_labelSize=obj.value("labelSize").toDouble(16);
    m_img=obj.value("img").toString("marker2.png");
    m_color=obj.value("color").toString("");
    m_radius=obj.value("radius").toDouble(16);
    m_aspectratio=obj.value("aspectratio").toDouble(1);
    m_rotation=obj.value("rotation").toDouble(0);
    m_light=obj.value("light").toString("circleSmoother.png");
    m_lightRadius=obj.value("lightRadius").toDouble(16);
    m_lightAspectratio=obj.value("lightAspectratio").toDouble(1);
    m_lightRotation=obj.value("lightRotation").toDouble(0);
    setX(obj.value("x").toDouble());
    setY(obj.value("y").toDouble());

    reload();
}
QJsonObject MapItem::toObj()
{
    QJsonObject obj;
    obj["name"]=m_name;
    obj["showName"]=m_showName;
    obj["labelSize"]=m_labelSize;
    obj["img"]=m_img;
    obj["radius"]=m_radius;
    obj["aspectratio"]=m_aspectratio;
    obj["rotation"]=m_rotation;
    obj["light"]=m_light;
    obj["lightRadius"]=m_lightRadius;
    obj["lightAspectratio"]=m_lightAspectratio;
    obj["lightRotation"]=m_lightRotation;
    obj["color"]=m_color;
    obj["x"]=x();
    obj["y"]=y();

    return obj;
}

bool MapItem::showName() const
{
    return m_showName;
}

void MapItem::setShowName(bool newShowName)
{
    m_showName = newShowName;
}

double MapItem::labelSize() const
{
    return m_labelSize;
}

void MapItem::setLabelSize(double newLabelSize)
{
    m_labelSize = newLabelSize;
}

QString MapItem::img() const
{
    return m_img;
}

void MapItem::setImg(const QString &newImg)
{
    m_img = newImg;
}

double MapItem::radius() const
{
    return m_radius;
}

void MapItem::setRadius(double newRadius)
{
    m_radius = newRadius;
}

double MapItem::aspectratio() const
{
    return m_aspectratio;
}

void MapItem::setAspectratio(double newAspectratio)
{
    m_aspectratio = newAspectratio;
}

double MapItem::rotation() const
{
    return m_rotation;
}

void MapItem::setRotation(double newRotation)
{
    m_rotation = newRotation;
}

QString MapItem::light() const
{
    return m_light;
}

void MapItem::setLight(const QString &newLight)
{
    m_light = newLight;
}

double MapItem::lightRadius() const
{
    return m_lightRadius;
}

void MapItem::setLightRadius(double newLightRadius)
{
    m_lightRadius = newLightRadius;
}

double MapItem::lightAspectratio() const
{
    return m_lightAspectratio;
}

void MapItem::setLightAspectratio(double newLightAspectratio)
{
    m_lightAspectratio = newLightAspectratio;
}

double MapItem::lightRotation() const
{
    return m_lightRotation;
}

void MapItem::setLightRotation(double newLightRotation)
{
    m_lightRotation = newLightRotation;
}

void MapItem::remove()
{
    Scene* currentScene= qobject_cast<Scene*>(scene());
    if(currentScene)
        currentScene->removeMarker(this);
    delete this;
}

QString MapItem::color() const
{
    return m_color;
}

void MapItem::setColor(const QString &newColor)
{
    m_color = newColor;
}
MapItem *MapItem::createPresentItem()
{
    MapItem* ret=new MapItem(toObj());
    ret->setFlag(QGraphicsItem::ItemIsSelectable,false);
    ret->setFlag(QGraphicsItem::ItemIsMovable,false);
    ret->setFlag(QGraphicsItem::ItemIsFocusable,false);
    ret->m_present=true;
    if(!m_showName)
    {
        ret->m_textItem->hide();
    }
    ret->setPos(pos());
    return ret;
}

void MapItem::reload()
{
    m_pixmapItem->setPixmap(createImage());
    m_pixmapItem->setOffset(-m_pixmapItem->boundingRect().width()/2,-m_pixmapItem->boundingRect().height()/2);
    m_pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    m_pixmapItem->setRotation(m_rotation);
    m_textItem->setHtml("<p style=\"color: white;background-color:rgba(0, 0, 0, 0.8);\">"+m_name+"</p>");

    if(!m_color.isEmpty())
    {
        QGraphicsColorizeEffect* effect=new QGraphicsColorizeEffect;
        effect->setColor(QColor::fromString(m_color));
        m_pixmapItem->setGraphicsEffect(effect);
    }

    m_textItem->setFont(QFont("Arial",m_labelSize));
    m_textItem->setPos(-m_textItem->boundingRect().width()/2,
                       -m_textItem->boundingRect().height()-(m_aspectratio*(m_radius-m_pixmapItem->boundingRect().height()/2))-2);


    Scene* currentScene= qobject_cast<Scene*>(scene());
    if(currentScene)
        currentScene->updateFogOfWar();
}

QPixmap MapItem::createLight()
{
     QPixmap pixmap("lights/"+m_light);
     if(pixmap.isNull()) return pixmap;
     return pixmap.scaled(m_lightRadius,m_lightAspectratio*m_lightRadius,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
}

QPixmap MapItem::createImage()
{
    QPixmap pixmap("img/"+m_img);
    if(pixmap.isNull()) return pixmap;
    return pixmap.scaled(m_radius,m_aspectratio*m_radius,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
}

QPixmap MapItem::icon()
{
    QPixmap ret=createImage();
    if(ret.isNull())
     ret=createLight();
    return ret;
}

QString MapItem::name() const
{
    return m_name;
}

void MapItem::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
}

QRectF MapItem::boundingRect() const
{
 return childrenBoundingRect();
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_present)
        return;
    if(isSelected())
    {
        painter->setPen(QPen(Qt::blue,1,Qt::DotLine));
    }
    else
    {

        painter->setPen(QPen(Qt::black,1,Qt::DotLine));
    }
    painter->drawRect(boundingRect().marginsRemoved(QMarginsF(2,2,2,2)));
}

QVariant MapItem::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if(change==ItemPositionChange)
    {

       Scene* currentScene= qobject_cast<Scene*>(scene());
       if(currentScene)
           currentScene->updateFogOfWar();
    }
    return value;
}
