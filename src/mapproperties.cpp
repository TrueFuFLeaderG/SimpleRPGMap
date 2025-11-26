#include "mapproperties.h"

#include <QFormLayout>

MapProperties::MapProperties(QWidget *parent)
    : QWidget{parent}
{
    QFormLayout* layout=new QFormLayout;
    setLayout(layout);
    m_gridSize->setMaximum(99999);
    m_gridXOffset->setRange(-99999,99999);
    m_gridYOffset->setRange(-99999,99999);
    layout->addRow(tr("Grid size"),m_gridSize);
    layout->addRow(tr("Grid x offset"),m_gridXOffset);
    layout->addRow(tr("Grid y offset"),m_gridYOffset);
    layout->addRow(tr("Show fog of war"),m_showFogOfWar);
    m_gridSize->setDisabled(true);
    m_gridXOffset->setDisabled(true);
    m_gridYOffset->setDisabled(true);
    m_showFogOfWar->setDisabled(true);
    m_drawWalls->setDisabled(true);
    m_drawWalls->setCheckable(true);
    layout->addRow(m_drawWalls);

    connect(m_gridSize,&QDoubleSpinBox::valueChanged,this,&MapProperties::updateScene);
    connect(m_gridXOffset,&QDoubleSpinBox::valueChanged,this,&MapProperties::updateScene);
    connect(m_gridYOffset,&QDoubleSpinBox::valueChanged,this,&MapProperties::updateScene);
    connect(m_showFogOfWar,&QCheckBox::stateChanged,this,&MapProperties::updateScene);
    connect(m_drawWalls,&QPushButton::clicked,this,&MapProperties::updateScene);

}

Scene *MapProperties::scene() const
{
    return m_scene;
}

void MapProperties::setScene(Scene *newScene)
{

    if(m_scene)
    {
        m_scene->setDrawWallsMode(false);
    }
    m_scene = newScene;
    m_gridSize->setDisabled(!m_scene);
    m_gridXOffset->setDisabled(!m_scene);
    m_gridYOffset->setDisabled(!m_scene);
    m_showFogOfWar->setDisabled(!m_scene);
    m_drawWalls->setDisabled(!m_scene);
    if(!m_scene)
    {
        return;
    }
    m_scene=0;
    m_gridSize->setValue(newScene->gridSize());
    m_gridXOffset->setValue(newScene->gridXOffset());
    m_gridYOffset->setValue(newScene->gridYOffset());
    m_showFogOfWar->setChecked(newScene->showFogOfWar());
    m_scene=newScene;
    m_drawWalls->setChecked(false);
}

void MapProperties::updateScene()
{

    if(!m_scene)
    {
        return;
    }
    m_scene->setShowFogOfWar(m_showFogOfWar->isChecked());
    m_scene->setGridSize(m_gridSize->value());
    m_scene->setGridXOffset(m_gridXOffset->value());
    m_scene->setGridYOffset(m_gridYOffset->value());
    m_scene->setDrawWallsMode(m_drawWalls->isChecked());
    m_scene->invalidate();
}
