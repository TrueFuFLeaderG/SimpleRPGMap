#ifndef SOUNDLIST_H
#define SOUNDLIST_H

#include <QTreeView>
#include <QWidget>
#include <QFileSystemModel>

class SoundList : public QTreeView
{
    Q_OBJECT
public:
    explicit SoundList(QWidget *parent = nullptr);

    QString path(const QModelIndex &index);
signals:
private:
    QFileSystemModel* m_model=new QFileSystemModel;
};

#endif // SOUNDLIST_H
