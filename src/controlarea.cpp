#include "controlarea.h"

#include <MapControl.h>
#include <QChildEvent>
#include <QFileInfo>
#include <QMdiSubWindow>

ControlArea::ControlArea()
{
    setTabsMovable(true);
    setViewMode(TabbedView);
    setTabsClosable(true);
}

void ControlArea::openMap(const QString &path)
{
    QFileInfo filepath(path);
    QString absFilePath=filepath.absoluteFilePath();
    QList<QMdiSubWindow *>  windows=subWindowList();
    for(int i=0;i<windows.size();i++)
    {
       if(windows[i]->property("filePath").toString()==absFilePath)
       {
           setActiveSubWindow(windows[i]);
           return;
       }
    }
    MapControl* controls= new MapControl(path);
    QMdiSubWindow* subwindow=addSubWindow(controls);
    subwindow->setWindowTitle( filepath.fileName());
    subwindow->setWindowFlag(Qt::WindowCloseButtonHint);
    subwindow->setAttribute(Qt::WA_DeleteOnClose);
    subwindow->setProperty("filePath",absFilePath);
    subwindow->show();
}

MapControl *ControlArea::currentMapControl()
{

    QMdiSubWindow* subwindow=currentSubWindow();
    if(!subwindow)
        return 0;
    return qobject_cast<MapControl*>(subwindow->widget());
}
