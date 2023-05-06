#ifndef MAPPROPERTIES_H
#define MAPPROPERTIES_H

#include <QCheckBox>
#include <QDoubleSpinBox>
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
    QCheckBox* m_showFogOfWar=new QCheckBox;

    Scene* m_scene;

};

#endif // MAPPROPERTIES_H
