#include "mainwindow.h"

#include <MarkerProperties.h>
#include <QComboBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QScreen>
#include <QToolBar>
#include <Scene.h>
#include <qjsonobject.h>
MainWindow* MainWindow::m_mainWindow=0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_mainWindow=this;
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
    box->addItem(tr("Manual Preview"));
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
    m_toolBar->addSeparator();

    QAction* saveA=m_toolBar->addAction(QIcon(),tr("save"));
    connect(saveA,&QAction::triggered,this,&MainWindow::saveState);
    QAction* loadA= m_toolBar->addAction(QIcon(),tr("load"));
    connect(loadA,&QAction::triggered,this,&MainWindow::loadState);

    connect(m_control,&QMdiArea::subWindowActivated,this,&MainWindow::upateSettings);
    startTimer(60000);
    load("./autosave.session.json",false);
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
    if(index==1)
    {

        m_presetMap=new MapControl("",true);
        m_presetMap->setWindowFlags(Qt::Window | Qt::WindowFullscreenButtonHint| Qt::WindowMaximizeButtonHint);
        m_presetMap->setLineWidth(0);
        m_presetMap->setFrameStyle(QFrame::NoFrame);
        m_presetMap->show();
        showMap();
        return;
    }
    QScreen* screen=QGuiApplication::screens().value(index-2);
    if(screen)
    {
        m_presetMap=new MapControl("",true);
        m_presetMap->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        m_presetMap->setLineWidth(0);
        m_presetMap->setFrameStyle(QFrame::NoFrame);
        m_presetMap->setScreen(screen);
        m_presetMap->setGeometry(screen->availableGeometry().adjusted(10,10,-20,-20));
        m_presetMap->showFullScreen();
        showMap();
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
}

void MainWindow::showMap()
{
    if(m_presetMap==0)
        return;
    m_presetMap->syncScene(m_control->currentMapControl());
}

void MainWindow::upateSettings()
{
    if(!m_control->currentMapControl())
        return;
    Scene* newScene=m_control->currentMapControl()->scene();
    connect(newScene,&Scene::focusItemChanged,m_markerProperties,&MarkerProperties::focusChanged);
    m_mapProperties->setScene(newScene);
}

void MainWindow::loadState()
{
    QString path=QFileDialog::getOpenFileName(this,tr("Load"),tr("."),tr("Session (*.session.json)"));
    if(path.isEmpty())
        return;
    load(path);
}

void MainWindow::saveState()
{
    QString path=QFileDialog::getSaveFileName(this,tr("Load"),tr("."),tr("Session (*.session.json)"));
    if(path.isEmpty())
        return;
    save(path);
}

void MainWindow::save(const QString &path)
{
    QJsonArray content;
    QList<QMdiSubWindow* > list= m_control->subWindowList();
    for(int i=0;i<list.size();i++)
    {
        MapControl* control=qobject_cast<MapControl*>(list[i]->widget());
        if(!control)
            continue;
        QJsonObject object;
        object["path"]=control->path();
        object["gridSize"]=control->scene()->gridSize();
        object["showFogOfWar"]=control->scene()->showFogOfWar();
        object["gridXOffset"]=control->scene()->gridXOffset();
        object["gridYOffset"]=control->scene()->gridYOffset();
        QList<MapItem *>  obj=control->scene()->mapItems();
        QJsonArray objects;
        for(int j=0;j<obj.size();j++)
        {
            objects.append(obj[j]->toObj());
        }
        object["objects"]=objects;
        content.append(object);
    }
    QJsonDocument doc(content);
    QFile f(path);
    if(f.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        f.write(doc.toJson());
        f.close();
    }
}

void MainWindow::load(const QString &path,bool message)
{

    m_control->closeAllSubWindows();
    QFile f(path);
    if(f.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QJsonParseError error;
        QJsonDocument doc=QJsonDocument::fromJson(f.readAll(),&error);
        f.close();
        if(error.error!=QJsonParseError::NoError)
        {

            if(message)
                QMessageBox::warning(this,tr("parser error offset %0 %1").arg(error.offset).arg(error.errorString()),
                                 tr("parser error offset %0 %1").arg(error.offset).arg(error.errorString()));
            return;
        }


        QJsonArray content=doc.array();
        for(int i=0;i<content.size();i++)
        {
            QJsonObject object=content[i].toObject();
            MapControl* control=m_control->openMap(object["path"].toString());
            control->scene()->setGridSize(object["gridSize"].toInt());
            control->scene()->setShowFogOfWar(object["showFogOfWar"].toBool());
            control->scene()->setGridXOffset(object["gridXOffset"].toInt());
            control->scene()->setGridYOffset(object["gridYOffset"].toInt());
            QJsonArray array=object["objects"].toArray();
            for(int j=0;j<array.size();j++)
            {
                control->scene()->addMarker(new MapItem(array[j].toObject()));
            }
            m_control->setActiveSubWindow(0);
            m_control->openMap(object["path"].toString());

        }
        upateSettings();

    }
    else
    {
        if(message)
            QMessageBox::warning(this,tr("can not open %0").arg(path),tr("can not open %0").arg(path));
    }
}

MapControl *MainWindow::presetMap() const
{
    return m_presetMap;
}

MarkerProperties *MainWindow::markerProperties() const
{
    return m_markerProperties;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    save("./autosave.session.json");
    QGuiApplication::quit();
}

void MainWindow::timerEvent(QTimerEvent */*event*/)
{
    save("./autosave.session.json");
}

