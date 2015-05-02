#ifndef EMODEL_H
#define EMODEL_H
#include <QString>
#include <QList>
#include "ediagram.h"
#include "eobject.h"

class EModel : public EObject
{
    QString name;
    QList<EDiagram*> diagrams;
public:
    EModel();
    void setName(QString name);
    QString getName();
    void setDiagrams(QList<EDiagram*> diagrams);
    QList<EDiagram*> getDiagrams();
    void addDiagram(EDiagram *diagram);
    EDiagram *getDiagram(int i);
    int diagramCount();
};

#endif // EMODEL_H
