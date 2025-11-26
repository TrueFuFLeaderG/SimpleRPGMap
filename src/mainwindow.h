#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mapcontrol.h"
#include "soundlist.h"
#include <ControlArea.h>
#include <MapList.h>
#include <MapProperties.h>
#include <MapShow.h>
#include <MarkerList.h>
#include <MarkerProperties.h>
#include <QAudioOutput>
#include <QMainWindow>
#include <QMediaPlayer>

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
    void setLoopSound(bool newLoopSound);
    void hideMap();
    void setPresetScreen(int index);
    void openMap(const QModelIndex& model);
    void showMap();
    void toggleFullscreen();
    void upateSettings();
    void loadState();
    void saveState();
    void playSound(const QModelIndex &index);
    void updateSound(int value);
private:
    void save(const QString& path);
    void load(const QString& path, bool message=true);
    ControlArea* m_control=new ControlArea;
    MarkerList* m_marker=new MarkerList;
    MapList* m_maplist=new MapList;
    SoundList* m_soundList=new SoundList;
    MarkerProperties* m_markerProperties=new MarkerProperties;
    MapProperties* m_mapProperties=new MapProperties;

    QMediaPlayer *m_player = new QMediaPlayer;
    QAudioOutput *m_audioOutput = new QAudioOutput;
    bool m_isUpdating=false;
    MapControl* m_presetMap=0;
    QToolBar* m_toolBar;
    Scene* m_scene=0;
    static MainWindow* m_mainWindow;
    QComboBox* m_box;
    QString m_lastSound;
    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);

    void timerEvent(QTimerEvent *event);
};
#endif // MAINWINDOW_H
