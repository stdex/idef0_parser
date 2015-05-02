#ifndef EDGE_H
#define EDGE_H
#include <QString>
#include <QList>
#include "eobject.h"

struct typeEdge{
    int id;
    QString name;
};

class edge : public EObject
{
    QString id;
    QString name;

    int start;
    QList<int*> ends;
    int nDiag;
    typeEdge *te;
public:
    edge();

    void setId(QString id);
    QString getId();
    void setName(QString name);
    QString getName();

    void setStart(int start);
    int getStart();

    void setEnds(QList<int*> ends);
    QList<int*> getEnds();
    void addEnd(int *end);
    int *getEnd(int i);
    int endCount();

    void setNDiag(int nDiag);
    int getNDiag();

    void setType(typeEdge *te);
    typeEdge *getType();
};

#endif // EDGE_H
