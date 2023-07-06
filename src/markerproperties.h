#ifndef MARKERPROPERTIES_H
#define MARKERPROPERTIES_H


#include "MapItem.h"
#include "qspinbox.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QWidget>
#include <qcombobox.h>

class QGraphicsItem;
class MarkerProperties : public QWidget
{
    Q_OBJECT
public:
    MarkerProperties();
public slots:
     void focusChanged(QGraphicsItem *newFocusItem, QGraphicsItem *oldFocusItem, Qt::FocusReason reason);
     void updateItem();
     void deleteCurrentItem();
     void saveCurrentItem();
     void duplicateCurrentItem();
private:
     MapItem* m_currentItem=0;


     QLineEdit* m_name= new QLineEdit;
     QCheckBox* m_showName= new QCheckBox;
     QCheckBox* m_removeBackground= new QCheckBox;
     QDoubleSpinBox* m_labelSize= new QDoubleSpinBox;
     QComboBox* m_img= new QComboBox;
     QComboBox* m_color= new QComboBox;
     QDoubleSpinBox* m_radius= new QDoubleSpinBox;
     QDoubleSpinBox* m_aspectratio= new QDoubleSpinBox;
     QSlider* m_rotation= new QSlider(Qt::Horizontal);
     QComboBox* m_light= new QComboBox;
     QDoubleSpinBox* m_lightRadius= new QDoubleSpinBox;
     QDoubleSpinBox* m_lightAspectratio= new QDoubleSpinBox;
     QSlider* m_lightRotation= new QSlider(Qt::Horizontal);


};

#endif // MARKERPROPERTIES_H
