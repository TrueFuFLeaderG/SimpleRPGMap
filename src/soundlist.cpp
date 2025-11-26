#include "soundlist.h"

SoundList::SoundList(QWidget *parent)
    : QTreeView{parent}
{
 setModel(m_model);
 m_model->setRootPath("./sound/");
 setRootIndex(m_model->index("./sound/"));
}


QString SoundList::path(const QModelIndex &index)
{
    return m_model->filePath(index);
}
