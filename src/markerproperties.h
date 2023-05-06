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
private:
     MapItem* m_currentItem=0;


     QLineEdit* m_name= new QLineEdit;
     QCheckBox* m_showName= new QCheckBox;
     QDoubleSpinBox* m_labelSize= new QDoubleSpinBox;
     QComboBox* m_img= new QComboBox;
     QDoubleSpinBox* m_radius= new QDoubleSpinBox;
     QDoubleSpinBox* m_aspectratio= new QDoubleSpinBox;
     QDoubleSpinBox* m_rotation= new QDoubleSpinBox;
     QComboBox* m_light= new QComboBox;
     QDoubleSpinBox* m_lightRadius= new QDoubleSpinBox;
     QDoubleSpinBox* m_lightAspectratio= new QDoubleSpinBox;
     QDoubleSpinBox* m_lightRotation= new QDoubleSpinBox;


};

#endif // MARKERPROPERTIES_H
