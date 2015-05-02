#include "edge.h"

edge::edge()
    : EObject()
{
    nDiag = -1;
}

void edge::setId(QString id)
{
    this->id = id;
}

QString edge::getId()
{
    return id;
}

void edge::setName(QString name)
{
    this->name = name;
}

QString edge::getName()
{
    return name;
}

void edge::setStart(int start)
{
    this->start = start;
}

int edge::getStart()
{
    return start;
}

void edge::setEnds(QList<int*> ends)
{
    this->ends = ends;
}

QList<int*> edge::getEnds()
{
    return ends;
}

void edge::addEnd(int *end)
{
    ends<< end;
}

int *edge::getEnd(int i)
{
    return ends.at(i);
}

int edge::endCount()
{
    return ends.size();
}

void edge::setNDiag(int nDiag)
{
    this->nDiag = nDiag;
}

int edge::getNDiag()
{
    return nDiag;
}

void edge::setType(typeEdge *te)
{
    this->te = te;
}

typeEdge *edge::getType()
{
    return te;
}
