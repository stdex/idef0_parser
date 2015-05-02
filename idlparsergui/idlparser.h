#ifndef IDLPARSER_H
#define IDLPARSER_H
#include <QString>
#include "ekit.h"
#include "QStringList"
#include "edge.h"

class IdlParser
{
    QString filename;
    QStringList startKeys;
    QStringList endKeys;
    QList<EObject*> elements;
    EKit *ekit;
    QList<edge*> edges;
    QList<typeEdge*> etypes;
public:
    IdlParser(QString filename);
    void parse();
    EKit *getEkit();
    QString clearName(QString param);
    QList<edge*> getEdges();
    typeEdge *getTypeFromId(int id);
};

#endif // IDLPARSER_H
