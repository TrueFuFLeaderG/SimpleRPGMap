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
    MarkerProperties *markerProperties() const;
    static MainWindow* mainWindow(){return m_mainWindow;}
    MapControl *presetMap() const;
    void updatePreviewOptions();
    MarkerList *marker() const;

    ControlArea *control() const;

public slots:
    void hideMap();
    void setPresetScreen(int index);
    void openMap(const QModelIndex& model);
    void showMap();
    void toggleFullscreen();
    void upateSettings();
    void loadState();
    void saveState();
private:
    void save(const QString& path);
    void load(const QString& path, bool message=true);
    ControlArea* m_control=new ControlArea;
    MarkerList* m_marker=new MarkerList;
    MapList* m_maplist=new MapList;
    MarkerProperties* m_markerProperties=new MarkerProperties;
    MapProperties* m_mapProperties=new MapProperties;

    bool m_isUpdating=false;
    MapControl* m_presetMap=0;
    QToolBar* m_toolBar;
    Scene* m_scene=0;
    static MainWindow* m_mainWindow;
    QComboBox* m_box;
    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);

    void timerEvent(QTimerEvent *event);
};
#endif // MAINWINDOW_H
