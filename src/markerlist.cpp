#include "markerlist.h"

#include <MapItem.h>
#include <QDir>

MarkerList::MarkerList(QWidget *parent)
    : QTreeWidget{parent}
{

    setHeaderLabels(QStringList()<<tr("Map items"));
    setDragDropMode(DragOnly);
    QFileInfoList files=QDir("./marker/").entryInfoList(QStringList()<<"*.json",QDir::Files);
    for(int i=0;i<files.size();i++)
    {
        MapItem mapItem(files[i].absoluteFilePath());
        QTreeWidgetItem* item=new QTreeWidgetItem(this,QStringList()<<mapItem.name());
        item->setIcon(0,QIcon(mapItem.icon()));
        item->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        item->setData(0,Qt::UserRole,files[i].absoluteFilePath());
    }
}
