#ifndef MAPPROPERTIES_H
#define MAPPROPERTIES_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QWidget>
#include <Scene.h>

class MapProperties : public QWidget
{
    Q_OBJECT
public:
    explicit MapProperties(QWidget *parent = nullptr);

    Scene *scene() const;
    void setScene(Scene *newScene);

public slots:
    void updateScene();
signals:
private:
    QDoubleSpinBox* m_gridSize=new QDoubleSpinBox;
    QDoubleSpinBox* m_gridXOffset=new QDoubleSpinBox;
    QDoubleSpinBox* m_gridYOffset=new QDoubleSpinBox;
    QCheckBox* m_showFogOfWar=new QCheckBox;
    QPushButton* m_drawWalls=new QPushButton(tr("Draw walls (hold shift polygons)"));

    Scene* m_scene=0;

};

#endif // MAPPROPERTIES_H
