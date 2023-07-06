#include "markerproperties.h"

#include <MainWindow.h>
#include <QFileDialog>
#include <QFileInfoList>
#include <QFormLayout>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <qdir.h>

MarkerProperties::MarkerProperties()
{

    QFormLayout* layout=new QFormLayout;
    setLayout(layout);

     m_labelSize->setMaximum(9999);
     m_radius->setMaximum(9999);
     m_aspectratio->setMaximum(9999);
     m_rotation->setMaximum(360);
     m_lightRadius->setMaximum(9999);
     m_lightAspectratio->setMaximum(9999);
     m_lightRotation->setMaximum(360);

    m_img->setEditable(true);
    m_light->setEditable(true);
     QFileInfoList files=QDir("./img/").entryInfoList(QDir::Files);
     for(int i=0;i<files.size();i++)
     {
         m_img->addItem(QIcon(files[i].absoluteFilePath()),files[i].fileName());
     }
     files=QDir("./lights/").entryInfoList(QDir::Files);
     for(int i=0;i<files.size();i++)
     {
         m_light->addItem(QIcon(files[i].absoluteFilePath()),files[i].fileName());
     }
     QStringList colorNames = QStringList()  <<
         "aliceblue"<<
         "antiquewhite"<<
         "aqua"<<
         "aquamarine"<<
         "azure"<<
         "beige"<<
         "bisque"<<
         "black"<<
         "blanchedalmond"<<
         "blue"<<
         "blueviolet"<<
         "brown"<<
         "burlywood"<<
         "cadetblue"<<
         "chartreuse"<<
         "chocolate"<<
         "coral"<<
         "cornflowerblue"<<
         "cornsilk"<<
         "crimson"<<
         "cyan"<<
         "darkblue"<<
         "darkcyan"<<
         "darkgoldenrod"<<
         "darkgray"<<
         "darkgreen"<<
         "darkgrey"<<
         "darkkhaki"<<
         "darkmagenta"<<
         "darkolivegreen"<<
         "darkorange"<<
         "darkorchid"<<
         "darkred"<<
         "darksalmon"<<
         "darkseagreen"<<
         "darkslateblue"<<
         "darkslategray"<<
         "darkslategrey"<<
         "darkturquoise"<<
         "darkviolet"<<
         "deeppink"<<
         "deepskyblue"<<
         "dimgray"<<
         "dimgrey"<<
         "dodgerblue"<<
         "firebrick"<<
         "floralwhite"<<
         "forestgreen"<<
         "fuchsia"<<
         "gainsboro"<<
         "ghostwhite"<<
         "gold"<<
         "goldenrod"<<
         "gray"<<
         "green"<<
         "greenyellow"<<
         "grey"<<
         "honeydew"<<
         "hotpink"<<
         "indianred"<<
         "indigo"<<
         "ivory"<<
         "khaki"<<
         "lavender"<<
         "lavenderblush"<<
         "lawngreen"<<
         "lemonchiffon"<<
         "lightblue"<<
         "lightcoral"<<
         "lightcyan"<<
         "lightgoldenrodyellow"<<
         "lightgray"<<
         "lightgreen"<<
         "lightgrey"<<
         "lightpink"<<
         "lightsalmon"<<
         "lightseagreen"<<
         "lightskyblue"<<
         "lightslategray"<<
         "lightslategrey"<<
         "lightsteelblue"<<
         "lightyellow"<<
         "lime"<<
         "limegreen"<<
         "linen"<<
         "magenta"<<
         "maroon"<<
         "mediumaquamarine"<<
         "mediumblue"<<
         "mediumorchid"<<
         "mediumpurple"<<
         "mediumseagreen"<<
         "mediumslateblue"<<
         "mediumspringgreen"<<
         "mediumturquoise"<<
         "mediumvioletred"<<
         "midnightblue"<<
         "mintcream"<<
         "mistyrose"<<
         "moccasin"<<
         "navajowhite"<<
         "navy"<<
         "oldlace"<<
         "olive"<<
         "olivedrab"<<
         "orange"<<
         "orangered"<<
         "orchid"<<
         "palegoldenrod"<<
         "palegreen"<<
         "paleturquoise"<<
         "palevioletred"<<
         "papayawhip"<<
         "peachpuff"<<
         "peru"<<
         "pink"<<
         "plum"<<
         "powderblue"<<
         "purple"<<
         "red"<<
         "rosybrown"<<
         "royalblue"<<
         "saddlebrown"<<
         "salmon"<<
         "sandybrown"<<
         "seagreen"<<
         "seashell"<<
         "sienna"<<
         "silver"<<
         "skyblue"<<
         "slateblue"<<
         "slategray"<<
         "slategrey"<<
         "snow"<<
         "springgreen"<<
         "steelblue"<<
         "tan"<<
         "teal"<<
         "thistle"<<
         "tomato"<<
         "transparent"
         "turquoise"<<
         "violet"<<
         "wheat"<<
         "white"<<
         "whitesmoke"<<
         "yellow"<<
         "yellowgreen";
    m_color->setEditable(true);
    m_color->addItem("");
    for(int i=0;i<colorNames.size();i++)
    {
        QPixmap p(16,16);
        p.fill(QColor(colorNames[i]));
        m_color->addItem(QIcon(p),colorNames[i]);
    }
    layout->addRow(tr("Name"),m_name);
    layout->addRow(tr("Show name"),m_showName);
    layout->addRow(tr("Hide background"),m_removeBackground);
    layout->addRow(tr("Label name size"),m_labelSize);
    layout->addRow(tr("Marker"),m_img);
    layout->addRow(tr("Color"),m_color);
    layout->addRow(tr("Marker size"),m_radius);
    layout->addRow(tr("Marker aspect ratio"),m_aspectratio);
    layout->addRow(tr("Marker rotation"),m_rotation);
    layout->addRow(tr("Light"),m_light);
    layout->addRow(tr("Light size"),m_lightRadius);
    layout->addRow(tr("Light aspect ratio"),m_lightAspectratio);
    layout->addRow(tr("Light rotation"),m_lightRotation);
    layout->addRow(tr("Light rotation"),m_lightRotation);

    QPushButton* button3=new QPushButton(tr("Duplicate item"));
    layout->addRow(button3);
    QPushButton* button2=new QPushButton(tr("Save this item as Template"));
    layout->addRow(button2);
    QPushButton* button=new QPushButton(tr("Delete this item"));
    layout->addRow(button);
    connect(button3,&QPushButton::clicked,this,&MarkerProperties::duplicateCurrentItem);
    connect(button2,&QPushButton::clicked,this,&MarkerProperties::saveCurrentItem);
    connect(button,&QPushButton::clicked,this,&MarkerProperties::deleteCurrentItem);
    connect(m_name,&QLineEdit::textChanged,this,&MarkerProperties::updateItem);
    connect(m_name,&QLineEdit::textChanged,this,&MarkerProperties::updateItem);
    connect(m_removeBackground,&QCheckBox::stateChanged,this,&MarkerProperties::updateItem);
    connect(m_showName,&QCheckBox::stateChanged,this,&MarkerProperties::updateItem);
    connect(m_labelSize,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_img,&QComboBox::currentIndexChanged,this,&MarkerProperties::updateItem);
    connect(m_color,&QComboBox::currentTextChanged,this,&MarkerProperties::updateItem);
    connect(m_radius,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_aspectratio,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_rotation,&QSlider::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_light,&QComboBox::currentIndexChanged,this,&MarkerProperties::updateItem);
    connect(m_lightRadius,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_lightAspectratio,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_lightRotation,&QSlider::valueChanged,this,&MarkerProperties::updateItem);

    setDisabled(true);
}

void MarkerProperties::focusChanged(QGraphicsItem *newFocusItem, QGraphicsItem */*oldFocusItem*/, Qt::FocusReason /*reason*/)
{
    MapItem* nextItem=qgraphicsitem_cast<MapItem*>(newFocusItem);
    if(nextItem!=0)
    {
        m_currentItem=0;
        setDisabled(false);
        m_name->setText(nextItem->name());
        m_showName->setChecked(nextItem->showName());
        m_removeBackground->setChecked(nextItem->removeBackground());
        m_labelSize->setValue(nextItem->labelSize());
        m_img->setCurrentText(nextItem->img());
        m_radius->setValue(nextItem->radius());
        m_aspectratio->setValue(nextItem->aspectratio());
        m_rotation->setValue(nextItem->rotation());
        m_light->setCurrentText(nextItem->light());
        m_lightRadius->setValue(nextItem->lightRadius());
        m_lightAspectratio->setValue(nextItem->lightAspectratio());
        m_lightRotation->setValue(nextItem->lightRotation());
        m_color->setCurrentText(nextItem->color());
        m_currentItem=nextItem;
    }
}

void MarkerProperties::updateItem()
{
    if(!m_currentItem)
        return;
    m_currentItem->setName(m_name->text());
    m_currentItem->setShowName(m_showName->isChecked());
    m_currentItem->setRemoveBackground(m_removeBackground->isChecked());
    m_currentItem->setLabelSize(m_labelSize->value());
    m_currentItem->setImg(m_img->currentText());
    m_currentItem->setRadius(m_radius->value());
    m_currentItem->setAspectratio(m_aspectratio->value());
    m_currentItem->setRotation(m_rotation->value());
    m_currentItem->setLight(m_light->currentText());
    m_currentItem->setLightRadius(m_lightRadius->value());
    m_currentItem->setLightAspectratio(m_lightAspectratio->value());
    m_currentItem->setLightRotation(m_lightRotation->value());
    m_currentItem->setColor(m_color->currentText());
    m_currentItem->reload();

}
void MarkerProperties::duplicateCurrentItem()
{

    if(!m_currentItem)
        return;
    Scene* scene=qobject_cast<Scene*>(m_currentItem->scene());
    if(!scene)
        return;
    MapItem* newMarker(new MapItem(m_currentItem->toObj()));
    newMarker->setPos(m_currentItem->pos()+QPointF(5,5));
    scene->addMarker(newMarker);
    scene->clearSelection();
    newMarker->setSelected(true);
    scene->setFocusItem(newMarker,Qt::MouseFocusReason);
}
void MarkerProperties::saveCurrentItem()
{
    QString path=QFileDialog::getSaveFileName(this,tr("Save template"),"./marker/","*.json");
    if(path.isEmpty())
        return;

    if(!m_currentItem)
        return;
    QJsonDocument doc(m_currentItem->toObj());
    QFile f(path);
    if(f.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        f.write(doc.toJson());
        f.close();
    }
    MainWindow::mainWindow()->marker()->updateList();
}

void MarkerProperties::deleteCurrentItem()
{
    if(m_currentItem)
        m_currentItem->remove();
    m_currentItem=0;
    setDisabled(true);
}
