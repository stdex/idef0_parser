#ifndef EARROWSEG_H
#define EARROWSEG_H
#include <QString>
#include <QPointF>
#include <QList>
#include "eobject.h"

class EArrowSeg : public EObject
{
    QString id;
    QString source;
    QList<QPointF> pathes;
    QString label;
    QPointF labelcoord;
    QString sink;
    QPointF squiggle_coordX;
    QPointF squiggle_coordY;

public:
    EArrowSeg();
    void setId(QString id);
    QString getId();
    void setSource(QString source);
    QString getSource();
    void setLabel(QString label);
    QString getLabel();
    void setLabelcoord(QPointF labelcoord);
    QPointF getLabelcoord();
    void setSink(QString sink);
    QString getSink();
    void setSquiggle_coordX(QPointF squiggle_coordX);
    QPointF getSquiggle_coordX();
    void setSquiggle_coordY(QPointF squiggle_coordY);
    QPointF getSquiggle_coordY();

    void setPathes(QList<QPointF> pathes);
    QList<QPointF> getPathes();
    void addPath(QPointF path);
    QPointF getPath(int i);
    int pathCount();

};

#endif // EARROWSEG_H
