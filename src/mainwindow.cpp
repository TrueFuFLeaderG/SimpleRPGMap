#include "mainwindow.h"

#include <MarkerProperties.h>
#include <QComboBox>
#include <QDockWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QToolBar>
#include <Scene.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(m_control);

    connect(m_maplist,&QAbstractItemView::doubleClicked,this,&MainWindow::openMap);
    QDockWidget* mapListDoc=new QDockWidget;
    mapListDoc->setWidget(m_maplist);
    addDockWidget(Qt::LeftDockWidgetArea,mapListDoc);



    QDockWidget* mapPropertiesDoc=new QDockWidget;
    mapPropertiesDoc->setWidget(m_mapProperties);
    addDockWidget(Qt::RightDockWidgetArea,mapPropertiesDoc);

    QDockWidget* markerListDoc=new QDockWidget;
    markerListDoc->setWidget(m_marker);
    addDockWidget(Qt::RightDockWidgetArea,markerListDoc);

    QDockWidget* markerPropertiesDoc=new QDockWidget;
    markerPropertiesDoc->setWidget(m_markerProperties);
    addDockWidget(Qt::RightDockWidgetArea,markerPropertiesDoc,Qt::Vertical);

    m_toolBar=addToolBar(tr("Controls"));
    QList<QScreen*> screens=QGuiApplication::screens();
    QComboBox* box=new QComboBox;
    box->addItem(tr("No Preview"));
    for(int i=0;i<screens.size();i++)
    {
        box->addItem(tr("Screen %0 %1").arg(i).arg(screens[i]->model()));
    }
    connect(box,&QComboBox::currentIndexChanged,this,&MainWindow::setPresetScreen);
    m_toolBar->addWidget(box);
    QAction* showCurrentMap=m_toolBar->addAction(QIcon(),tr("Show Current Map"));
    connect(showCurrentMap,&QAction::triggered,this,&MainWindow::showMap);
    QAction* hideMap= m_toolBar->addAction(QIcon(),tr("Hide Map"));
    connect(hideMap,&QAction::triggered,this,&MainWindow::hideMap);

    connect(m_control,&QMdiArea::subWindowActivated,this,&MainWindow::upateSettings);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setPresetScreen(int index)
{
    if(m_presetMap)
    {
        m_presetMap->deleteLater();
        m_presetMap=0;
    }
    QScreen* screen=QGuiApplication::screens().value(index-1);
    if(screen)
    {
        m_presetMap=new MapControl(true);
        m_presetMap->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        m_presetMap->setLineWidth(0);
        m_presetMap->setFrameStyle(QFrame::NoFrame);
        m_presetMap->setScreen(screen);
        m_presetMap->setGeometry(screen->availableGeometry());
        m_presetMap->showFullScreen();
    }
}

void MainWindow::openMap(const QModelIndex &index)
{
    m_control->openMap(m_maplist->path(index));
}


void MainWindow::hideMap()
{
    if(m_presetMap==0)
        return;
    m_presetMap->syncScene(0);
    m_presetMap->resetTransform();
}

void MainWindow::showMap()
{
    if(m_presetMap==0)
        return;
    m_presetMap->syncScene(m_control->currentMapControl());
    m_presetMap->resetTransform();
}

void MainWindow::upateSettings()
{
    if(m_scene)
    {

    }
    Scene* newScene=m_control->currentMapControl()->scene();
    connect(newScene,&Scene::focusItemChanged,m_markerProperties,&MarkerProperties::focusChanged);
    m_mapProperties->setScene(newScene);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    QGuiApplication::quit();
}

