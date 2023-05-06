#ifndef MARKERLIST_H
#define MARKERLIST_H

#include <QTreeWidget>
#include <QWidget>

class MarkerList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MarkerList(QWidget *parent = nullptr);

signals:

};

#endif // MARKERLIST_H
