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
#include <QVBoxLayout>
#include <Scene.h>
#include <qjsonobject.h>

void MainWindow::updateSound(int value)
{
    // Optional: set volume (0.0 to 1.0)
    m_audioOutput->setVolume(value/100.0);
}
MainWindow* MainWindow::m_mainWindow=0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_mainWindow=this;
    setCentralWidget(m_control);

    connect(m_maplist,&QAbstractItemView::doubleClicked,this,&MainWindow::openMap);
    connect(m_soundList,&QAbstractItemView::doubleClicked,this,&MainWindow::playSound);

    QDockWidget* mapListDoc=new QDockWidget;
    mapListDoc->setWidget(m_maplist);
    addDockWidget(Qt::LeftDockWidgetArea,mapListDoc);

    QDockWidget* soundListDoc=new QDockWidget(tr("Sound"));
    QWidget* sound=new QWidget;
    QSlider* slider = new QSlider(Qt::Horizontal);
    QCheckBox* box=new QCheckBox(tr("loop"));
    slider->setRange(0,100);
    slider->setValue(50);
    sound->setLayout(new QVBoxLayout);
    sound->layout()->addWidget(box);
    sound->layout()->addWidget(slider);
    sound->layout()->addWidget(m_soundList);
    connect(slider,&QSlider::valueChanged,this,&MainWindow::updateSound);
    connect(box,&QCheckBox::toggled,this,&MainWindow::setLoopSound);
    soundListDoc->setWidget(sound);
    addDockWidget(Qt::LeftDockWidgetArea,soundListDoc,Qt::Vertical);


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
    m_box=new QComboBox;
    connect(m_box,&QComboBox::currentIndexChanged,this,&MainWindow::setPresetScreen);
    m_toolBar->addWidget(m_box);
    updatePreviewOptions();
    QAction* showCurrentMap=m_toolBar->addAction(QIcon(),tr("Show Current Map"));

    connect(m_toolBar->addAction(QIcon(),tr("Toggle Fullscreen")),&QAction::triggered,this,&MainWindow::toggleFullscreen);
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



    m_player->setAudioOutput(m_audioOutput);
    load("./autosave.session.json",false);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setPresetScreen(int index)
{
    if(m_isUpdating)
        return;

    updatePreviewOptions();
    if(m_presetMap)
    {
        m_presetMap->deleteLater();
        m_presetMap=0;
    }
    if(index==1)
    {

        m_presetMap=new MapControl("",true);
        m_presetMap->setWindowFlags(Qt::Window |Qt::CustomizeWindowHint|Qt::WindowTitleHint);
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
        m_presetMap->setGeometry(screen->availableGeometry().adjusted(200,200,-400,-400));
        m_presetMap->showFullScreen();
        showMap();
    }
}

void MainWindow::openMap(const QModelIndex &index)
{
    m_control->openMap(m_maplist->path(index));
}
void MainWindow::playSound(const QModelIndex &index)
{
    m_lastSound=m_soundList->path(index);
    m_player->setSource(QUrl::fromLocalFile(m_lastSound));



    // Start playback
    m_player->play();

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

void MainWindow::toggleFullscreen()
{
    if(m_presetMap==0)
        return;
    if(m_presetMap->isFullScreen())
        m_presetMap->showNormal();
    else
       m_presetMap->showFullScreen();
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
        object["vx"]=control->scene()->viewportItem()->x();
        object["vy"]=control->scene()->viewportItem()->y();
        object["vw"]=control->scene()->viewportItem()->width();
        object["vh"]=control->scene()->viewportItem()->height();


        QList<WallItem *>  walls=control->scene()->walls();
        QJsonArray wallsJson;
        for(int j=0;j<walls.size();j++)
        {
            QJsonArray wall;
            wall.append(walls[j]->pos().x());
            wall.append(walls[j]->pos().y());
            QList<QPointF> points=walls[j]->rawPoints();
            for(QPointF point:points)
            {
                wall.append(point.x());
                wall.append(point.y());
            }
            wallsJson.append(wall);
        }
        object["walls"]=wallsJson;

        QList<MapItem *>  obj=control->scene()->mapItems();
        QJsonArray objects;
        for(int j=0;j<obj.size();j++)
        {
            objects.append(obj[j]->toObj());
        }
        object["objects"]=objects;
        content.append(object);
    }
    QJsonObject saveFile;
    saveFile["Maps"]=content;
    if(m_control->currentMapControl())
        saveFile["OpendMap"]=m_control->currentMapControl()->path();
    QJsonDocument doc(saveFile);
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

        QJsonObject file=doc.object();

        QJsonArray content=file["Maps"].toArray();
        for(int i=0;i<content.size();i++)
        {
            QJsonObject object=content[i].toObject();
            MapControl* control=m_control->openMap(object["path"].toString());
            control->scene()->setGridSize(object["gridSize"].toInt());
            control->scene()->setShowFogOfWar(object["showFogOfWar"].toBool());
            control->scene()->setGridXOffset(object["gridXOffset"].toInt());
            control->scene()->setGridYOffset(object["gridYOffset"].toInt());

            double vx =object["vx"].toDouble();
            double vy =object["vy"].toDouble();
            double vw =object["vw"].toDouble();
            double vh =object["vh"].toDouble();
            if(vw!=0.&&vh!=0.)
                control->scene()->viewportItem()->setRect(vx,vy,vw,vh);

            QJsonArray array=object["objects"].toArray();
            for(int j=0;j<array.size();j++)
            {
                control->scene()->addMarker(new MapItem(array[j].toObject()));
            }

            QJsonArray wallArray=object["walls"].toArray();
            for(int j=0;j<wallArray.size();j++)
            {
                QJsonArray wall=wallArray[j].toArray();
                QList<QPointF> points;
                for(int i=2;i<wall.size();i+=2)
                {
                    points<<QPointF(wall.at(i).toDouble(),wall.at(i+1).toDouble());
                }
                control->scene()->addWall(wall.at(0).toDouble(),wall.at(1).toDouble(),points);
            }


            control->scene()->updateFogOfWar();
            m_control->setActiveSubWindow(0);
            m_control->openMap(object["path"].toString());

        }
        QString opend=file["OpendMap"].toString();
        if(!opend.isEmpty())
            m_control->openMap(opend);
        upateSettings();

    }
    else
    {
        if(message)
            QMessageBox::warning(this,tr("can not open %0").arg(path),tr("can not open %0").arg(path));
    }
}



void MainWindow::setLoopSound(bool newLoopSound)
{
    if(newLoopSound)
        m_player->setLoops(QMediaPlayer::Infinite);
    else
    {
        m_player->stop();
        m_player->setLoops(QMediaPlayer::Once);
    }
}

ControlArea *MainWindow::control() const
{
    return m_control;
}

MarkerList *MainWindow::marker() const
{
    return m_marker;
}

MapControl *MainWindow::presetMap() const
{
    return m_presetMap;
}

void MainWindow::updatePreviewOptions()
{
    m_isUpdating=true;
    QString text=m_box->currentText();
    m_box->clear();
    QList<QScreen*> screens=QGuiApplication::screens();
    m_box->addItem(tr("No Preview"));
    m_box->addItem(tr("Manual Preview"));
    for(int i=0;i<screens.size();i++)
    {
        m_box->addItem(tr("Screen %0 %1 %2 %3").arg(i).arg(screens[i]->name()).arg(screens[i]->model()).arg(screens[i]->manufacturer()));
    }
    m_box->setCurrentText(text);
    m_isUpdating=false;
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

