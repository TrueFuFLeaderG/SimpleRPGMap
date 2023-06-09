#ifndef CONTROLAREA_H
#define CONTROLAREA_H

#include <QMdiArea>
#include <QWidget>
#include <QHash>
#include <QMdiSubWindow>
#include <MapControl.h>

class ControlArea : public QMdiArea
{
    Q_OBJECT
public:
    ControlArea();
    MapControl* openMap(const QString& path);

    MapControl* currentMapControl();

};

#endif // CONTROLAREA_H
