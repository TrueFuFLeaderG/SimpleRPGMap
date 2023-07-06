#include "markerlist.h"

#include <MapItem.h>
#include <QDir>
#include <QFileIconProvider>

MarkerList::MarkerList(QWidget *parent)
    : QTreeWidget{parent}
{

    setHeaderLabels(QStringList()<<tr("Map items"));
    setDragDropMode(DragOnly);
    updateList();
}

void MarkerList::updateList()
{

    clear();
    addFolder(invisibleRootItem(), QDir("./marker/").entryInfoList( QDir::NoDotAndDotDot|QDir::AllEntries,QDir::DirsFirst|QDir::Name));
}

void MarkerList::addFolder(QTreeWidgetItem *parent, const QFileInfoList &files)
{
    for(int i=0;i<files.size();i++)
    {
        if(files[i].isDir())
        {

            QTreeWidgetItem* item=new QTreeWidgetItem(parent,QStringList()<<files[i].fileName());
            item->setIcon(0,QIcon(QFileIconProvider().icon(files[i])));
            item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);

            addFolder(item, QDir(files[i].absoluteFilePath()).entryInfoList( QDir::NoDotAndDotDot|QDir::AllEntries,QDir::DirsFirst|QDir::Name));
        }
        if(files[i].fileName().endsWith(".json"))
        {
            MapItem mapItem(files[i].absoluteFilePath());
            QTreeWidgetItem* item=new QTreeWidgetItem(parent,QStringList()<<mapItem.name());
            item->setIcon(0,QIcon(mapItem.icon()));
            item->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
            item->setData(0,Qt::UserRole,files[i].absoluteFilePath());
        }
    }
}
