#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mapcontrol.h"
#include <ControlArea.h>
#include <MapList.h>
#include <MapProperties.h>
#include <MapShow.h>
#include <MarkerList.h>
#include <MarkerProperties.h>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void hideMap();
private slots:
    void setPresetScreen(int index);
    void openMap(const QModelIndex& model);
    void showMap();
    void upateSettings();
private:
    ControlArea* m_control=new ControlArea;
    MarkerList* m_marker=new MarkerList;
    MapList* m_maplist=new MapList;
    MapShow* m_display=new MapShow;
    MarkerProperties* m_markerProperties=new MarkerProperties;
    MapProperties* m_mapProperties=new MapProperties;

    MapControl* m_presetMap=0;
    QToolBar* m_toolBar;
    Scene* m_scene=0;


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
