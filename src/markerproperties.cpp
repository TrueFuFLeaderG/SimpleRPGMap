#include "markerproperties.h"

#include <QFileInfoList>
#include <QFormLayout>
#include <QIcon>
#include <QPushButton>
#include <qdir.h>

MarkerProperties::MarkerProperties()
{

    QFormLayout* layout=new QFormLayout;
    setLayout(layout);

     m_labelSize->setMaximum(9999);
     m_radius->setMaximum(9999);
     m_aspectratio->setMaximum(9999);
     m_rotation->setMaximum(9999);
     m_lightRadius->setMaximum(9999);
     m_lightAspectratio->setMaximum(9999);
     m_lightRotation->setMaximum(9999);

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


    layout->addRow(tr("Name"),m_name);
    layout->addRow(tr("Show name"),m_showName);
    layout->addRow(tr("Label name size"),m_labelSize);
    layout->addRow(tr("Marker"),m_img);
    layout->addRow(tr("Marker size"),m_radius);
    layout->addRow(tr("Marker aspect ratio"),m_aspectratio);
    layout->addRow(tr("Marker rotation"),m_rotation);
    layout->addRow(tr("Light"),m_light);
    layout->addRow(tr("Light size"),m_lightRadius);
    layout->addRow(tr("Light aspect ratio"),m_lightAspectratio);
    layout->addRow(tr("Light rotation"),m_lightRotation);
    layout->addRow(tr("Light rotation"),m_lightRotation);

    QPushButton* button=new QPushButton(tr("Delete this item"));
    layout->addRow(button);
    connect(button,&QPushButton::clicked,this,&MarkerProperties::deleteCurrentItem);
    connect(m_name,&QLineEdit::textChanged,this,&MarkerProperties::updateItem);
    connect(m_name,&QLineEdit::textChanged,this,&MarkerProperties::updateItem);
    connect(m_showName,&QCheckBox::stateChanged,this,&MarkerProperties::updateItem);
    connect(m_labelSize,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_img,&QComboBox::currentIndexChanged,this,&MarkerProperties::updateItem);
    connect(m_radius,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_aspectratio,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_rotation,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_light,&QComboBox::currentIndexChanged,this,&MarkerProperties::updateItem);
    connect(m_lightRadius,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_lightAspectratio,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);
    connect(m_lightRotation,&QDoubleSpinBox::valueChanged,this,&MarkerProperties::updateItem);

    setDisabled(true);
}

void MarkerProperties::focusChanged(QGraphicsItem *newFocusItem, QGraphicsItem *oldFocusItem, Qt::FocusReason reason)
{
    MapItem* nextItem=qgraphicsitem_cast<MapItem*>(newFocusItem);
    if(nextItem!=0)
    {
        m_currentItem=0;
        setDisabled(false);
        m_name->setText(nextItem->name());
        m_showName->setChecked(nextItem->showName());
        m_labelSize->setValue(nextItem->labelSize());
        m_img->setCurrentText(nextItem->img());
        m_radius->setValue(nextItem->radius());
        m_aspectratio->setValue(nextItem->aspectratio());
        m_rotation->setValue(nextItem->rotation());
        m_light->setCurrentText(nextItem->light());
        m_lightRadius->setValue(nextItem->lightRadius());
        m_lightAspectratio->setValue(nextItem->lightAspectratio());
        m_lightRotation->setValue(nextItem->lightRotation());
        m_currentItem=nextItem;
    }
}

void MarkerProperties::updateItem()
{
    if(!m_currentItem)
        return;
    m_currentItem->setName(m_name->text());
    m_currentItem->setShowName(m_showName->isChecked());
    m_currentItem->setLabelSize(m_labelSize->value());
    m_currentItem->setImg(m_img->currentText());
    m_currentItem->setRadius(m_radius->value());
    m_currentItem->setAspectratio(m_aspectratio->value());
    m_currentItem->setRotation(m_rotation->value());
    m_currentItem->setLight(m_light->currentText());
    m_currentItem->setLightRadius(m_lightRadius->value());
    m_currentItem->setLightAspectratio(m_lightAspectratio->value());
    m_currentItem->setLightRotation(m_lightRotation->value());

    m_currentItem->reload();

}

void MarkerProperties::deleteCurrentItem()
{
    m_currentItem->remove();
}
