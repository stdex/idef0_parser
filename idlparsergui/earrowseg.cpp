#include "earrowseg.h"

EArrowSeg::EArrowSeg()
    : EObject()
{
}

void EArrowSeg::setId(QString id)
{
    this->id = id;
}

QString EArrowSeg::getId()
{
    return id;
}

void EArrowSeg::setSource(QString source)
{
    this->source = source;
}

QString EArrowSeg::getSource()
{
    return source;
}

void EArrowSeg::setLabel(QString label)
{
    this->label = label;
}

QString EArrowSeg::getLabel()
{
    return label;
}

void EArrowSeg::setLabelcoord(QPointF labelcoord)
{
    this->labelcoord = labelcoord;
}

QPointF EArrowSeg::getLabelcoord()
{
    return labelcoord;
}

void EArrowSeg::setSink(QString sink)
{
    this->sink = sink;
}

QString EArrowSeg::getSink()
{
    return sink;
}

void EArrowSeg::setSquiggle_coordX(QPointF squiggle_coordX)
{
    this->squiggle_coordX = squiggle_coordX;
}

QPointF EArrowSeg::getSquiggle_coordX()
{
    return squiggle_coordX;
}

void EArrowSeg::setSquiggle_coordY(QPointF squiggle_coordY)
{
    this->squiggle_coordY = squiggle_coordY;
}

QPointF EArrowSeg::getSquiggle_coordY()
{
    return squiggle_coordY;
}

void EArrowSeg::setPathes(QList<QPointF> pathes)
{
    this->pathes = pathes;
}

QList<QPointF> EArrowSeg::getPathes()
{
    return pathes;
}

void EArrowSeg::addPath(QPointF path)
{
    pathes<<path;
}

QPointF EArrowSeg::getPath(int i)
{
    return pathes.at(i);
}

int EArrowSeg::pathCount()
{
    return pathes.size();
}
