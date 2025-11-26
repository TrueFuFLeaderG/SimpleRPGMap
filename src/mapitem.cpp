#include "mapitem.h"

#include <QFile>
#include <QGraphicsColorizeEffect>
#include <QGraphicsSceneMouseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <Scene.h>
#include <qpainter.h>

MapItem::MapItem(const QString &path)
{

    m_pixmapItem=new QGraphicsPixmapItem(this);
    m_textItem= new QGraphicsTextItem(this);
    m_scalItem=new ScaleItem(this);
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
    m_scalItem=new ScaleItem(this);
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
    m_removeBackground=obj.value("removeBackground").toBool(true);
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
    m_notes=obj.value("notes").toString();
    m_blockLight = obj.value("blockLight").toBool(false);
    setX(obj.value("x").toDouble());
    setY(obj.value("y").toDouble());

    reload();
}
QJsonObject MapItem::toObj()
{
    QJsonObject obj;
    obj["name"]=m_name;
    obj["removeBackground"]=m_removeBackground;
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
    obj["notes"] = m_notes;
    obj["blockLight"] = m_blockLight;
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

bool MapItem::removeBackground() const
{
    return m_removeBackground;
}

void MapItem::setRemoveBackground(bool newRemoveBackground)
{
    m_removeBackground = newRemoveBackground;
}

QString MapItem::notes() const
{
    return m_notes;
}

void MapItem::setNotes(const QString &newNotes)
{
    m_notes = newNotes;
}

bool MapItem::blockLight() const
{
    return m_blockLight;
}

void MapItem::setBlockLight(bool newBlockLight)
{
    m_blockLight = newBlockLight;
}

bool MapItem::present() const
{
    return m_present;
}

bool MapItem::scaleable()
{
    return !m_img.isEmpty();
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
    QPixmap img=createImage();
    if(m_removeBackground)
    {
        img.setMask(QPixmap::fromImage(img.toImage().convertedTo(QImage::Format_Indexed8,Qt::ColorOnly|Qt::ThresholdDither)).createHeuristicMask());
    }
    m_pixmapItem->setPixmap(img);
    m_pixmapItem->setOffset(-m_pixmapItem->boundingRect().width()/2,-m_pixmapItem->boundingRect().height()/2);
    m_pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    m_pixmapItem->setRotation(m_rotation);
    m_textItem->setHtml("<p style=\"color: white;background-color:rgba(0, 0, 0, 0.8);\">&nbsp;"+m_name+"&nbsp;</p>");

    if(!m_color.isEmpty())
    {
        QGraphicsColorizeEffect* effect=new QGraphicsColorizeEffect;
        effect->setColor(QColor::fromString(m_color));
        m_pixmapItem->setGraphicsEffect(effect);
    }
    else
    {
        m_pixmapItem->setGraphicsEffect(0);
    }

    m_textItem->setFont(QFont("Arial",m_labelSize));
    m_textItem->setPos(-m_textItem->boundingRect().width()/2,
                       -m_textItem->boundingRect().height()-(m_aspectratio*(m_radius-m_pixmapItem->boundingRect().height()/2))-2);


    Scene* currentScene= qobject_cast<Scene*>(scene());
    if(currentScene)
        currentScene->updateFogOfWar();
    m_scalItem->updatePos();
    m_scalItem->update();
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

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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

ScaleItem::ScaleItem(MapItem *item):QGraphicsItem(item)
{
    setZValue(1000);
    m_item=item;
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    updatePos();
}

QRectF ScaleItem::boundingRect() const
{
    return QRectF(-1,-1,m_scaleRect.width()+m_penWidth*2,m_scaleRect.height()+m_penWidth*2);
}
void ScaleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_item->present() || !m_item->scaleable())
        return;

    if(m_state)
        painter->setBrush(QBrush(Qt::red));
    else
        painter->setBrush(QBrush(Qt::gray));

    painter->drawEllipse(m_scaleRect);


    painter->drawLine(m_scaleRect.x()+(m_scaleRect.width()/4.)+1,
                      m_scaleRect.y()+(m_scaleRect.height()/4.)+1,
                      m_scaleRect.x()+(m_scaleRect.width()*(3./4.))-1,
                      m_scaleRect.y()+(m_scaleRect.height()*(3./4.))-1);

    painter->drawLine(m_scaleRect.x()+(m_scaleRect.width()/4.), m_scaleRect.y()+(m_scaleRect.height()/4.),m_scaleRect.x()+(m_scaleRect.width()/4.)+3, m_scaleRect.y()+(m_scaleRect.height()/4.));
    painter->drawLine(m_scaleRect.x()+(m_scaleRect.width()/4.), m_scaleRect.y()+(m_scaleRect.height()/4.),m_scaleRect.x()+(m_scaleRect.width()/4.), m_scaleRect.y()+(m_scaleRect.height()/4.)+3);

    painter->drawLine(m_scaleRect.x()+(m_scaleRect.width()*(3./4.)),m_scaleRect.y()+(m_scaleRect.height()*(3./4.)),m_scaleRect.x()+(m_scaleRect.width()*(3./4.))-3,m_scaleRect.y()+(m_scaleRect.height()*(3./4.)));
    painter->drawLine(m_scaleRect.x()+(m_scaleRect.width()*(3./4.)),m_scaleRect.y()+(m_scaleRect.height()*(3./4.)),m_scaleRect.x()+(m_scaleRect.width()*(3./4.)),m_scaleRect.y()+(m_scaleRect.height()*(3./4.))-3);

}

void ScaleItem::updatePos()
{
    setPos(m_item->boundingRect().height()/-2,m_item->boundingRect().width()/-2);
}



void ScaleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_item->present() || !m_item->scaleable())
        return;
    m_state=true;
}

void ScaleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if(m_item->present() || !m_item->scaleable())
        return;
    QPointF delta=event->lastPos()-event->pos();
    if(m_state)
    {
        prepareGeometryChange();
        if(qAbs( delta.x())>qAbs(delta.y()))
        {
            m_item->setRadius(m_item->radius()+delta.x());
        }
        else
        {
            m_item->setRadius(m_item->radius()+delta.y());
        }
        if(m_item->radius()<1)
            m_item->setRadius(1);
        m_item->reload();
        scene()->setFocusItem(m_item,Qt::OtherFocusReason);
        updatePos();
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void ScaleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_item->present() || !m_item->scaleable())
        return;
    m_state=false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void ScaleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{

    if(m_item->present() || !m_item->scaleable())
        return;
    m_state=false;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void ScaleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(m_item->present() || !m_item->scaleable())
        return;

    m_state=false;
    QGraphicsItem::hoverLeaveEvent(event);
}
