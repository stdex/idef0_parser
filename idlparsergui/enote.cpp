#include "enote.h"

ENote::ENote()
    :EObject()
{
}

void ENote::setId(QString id)
{
    this->id = id;
}

QString ENote::getId()
{
    return id;
}

void ENote::setCoord(QPointF coord)
{
    this->coord = coord;
}

QPointF ENote::getCoord()
{
    return coord;
}

void ENote::setText(QString text)
{
    this->text = text;
}

QString ENote::getText()
{
    return text;
}
