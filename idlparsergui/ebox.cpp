#include "ebox.h"

EBox::EBox()
    : EObject()
{
}

void EBox::setId(QString id)
{
    this->id = id;
}

QString EBox::getId()
{
    return id;
}

void EBox::setName(QString name)
{
    this->name = name;
}

QString EBox::getName()
{
    return name;
}

void EBox::setCoordX(QPointF coordX)
{
    this->coordX = coordX;
}

QPointF EBox::getCoordX()
{
    return coordX;
}

void EBox::setCoordY(QPointF coordY)
{
    this->coordY = coordY;
}

QPointF EBox::getCoordY()
{
    return coordY;
}

void EBox::setDetails(QString details)
{
    this->details = details;
}

QString EBox::getDetails()
{
    return details;
}

