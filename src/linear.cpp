#include "linear.h"

#include <QFont>
#include <Scene.h>

Linear::Linear()
{

    m_line->setPen(QPen(QColor(255,0,0,100),10,Qt::DashLine));
    m_circle->setPen(QPen(QColor(255,0,0,100),5,Qt::DashLine));
    addToGroup(m_line);
    addToGroup(m_text);
    addToGroup(m_circle);
    m_text->setFont(QFont("Arial",16));
}

void Linear::setStartPoint(const QPointF &p)
{
    if(p.x()<0)
        hide();
    show();
    setPos(p);
}

void Linear::setEndPoint(const QPointF &p)
{
    m_line->setLine(QLineF(QPointF(0,0),p-pos()));
    double rad= m_line->line().length();
    m_circle->setRect(-rad,-rad,rad*2,rad*2);
    updateText();
}

void Linear::updateText()
{
    Scene* sc=qobject_cast<Scene*>(scene());
    if(!sc)return;
    m_text->setHtml(QString("<p style=\"color: white;background-color:rgba(0, 0, 0, 0.8);\">&nbsp;%0&nbsp;</p>").arg(m_line->line().length()/sc->gridSize()));
}
