#ifndef MAPLIST_H
#define MAPLIST_H

#include <QTreeView>
#include <QWidget>
#include <QFileSystemModel>

class MapList : public QTreeView
{
    Q_OBJECT
public:
    explicit MapList(QWidget *parent = nullptr);

    QString path(const QModelIndex &index);
signals:
private:
    QFileSystemModel* m_model=new QFileSystemModel;
};

#endif // MAPLIST_H
