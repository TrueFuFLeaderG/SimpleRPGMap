#include "linear.h"

#include <QFont>
#include <Scene.h>

Linear::Linear()
{

    m_line->setPen(QPen(QColor(255,0,0,100),10,Qt::DashLine));
    addToGroup(m_line);
    addToGroup(m_text);
    m_text->setFont(QFont("Arial",16));
}

void Linear::setStartPoint(const QPointF &p)
{
    setPos(p);
}

void Linear::setEndPoint(const QPointF &p)
{
    m_line->setLine(QLineF(QPointF(0,0),p-pos()));
    updateText();
}

void Linear::updateText()
{
    Scene* sc=qobject_cast<Scene*>(scene());
    if(!sc)return;
    m_text->setHtml(QString("<p style=\"color: white;background-color:rgba(0, 0, 0, 0.8);\">%0</p>").arg(m_line->line().length()/sc->gridSize()));
}
