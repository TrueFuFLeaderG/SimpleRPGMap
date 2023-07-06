#ifndef MARKERLIST_H
#define MARKERLIST_H

#include <QFileInfoList>
#include <QTreeWidget>
#include <QWidget>

class MarkerList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MarkerList(QWidget *parent = nullptr);
    void updateList();
signals:
private:

    void addFolder(QTreeWidgetItem* parent,const QFileInfoList& files);

};

#endif // MARKERLIST_H
