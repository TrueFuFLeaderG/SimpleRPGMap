#include "maplist.h"

MapList::MapList(QWidget *parent)
    : QTreeView{parent}
{
 setModel(m_model);
 m_model->setRootPath("./maps/");
 setRootIndex(m_model->index("./maps/"));
}


QString MapList::path(const QModelIndex &index)
{
    return m_model->filePath(index);
}
