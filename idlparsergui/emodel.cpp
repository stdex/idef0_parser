#include "emodel.h"

EModel::EModel()
    : EObject()
{
}

void EModel::setName(QString name)
{
    this->name = name;
}

QString EModel::getName()
{
    return name;
}

void EModel::setDiagrams(QList<EDiagram*> diagrams)
{
    this->diagrams = diagrams;
}

QList<EDiagram*> EModel::getDiagrams()
{
    return diagrams;
}

void EModel::addDiagram(EDiagram *diagram)
{
    diagrams<<diagram;
}

EDiagram *EModel::getDiagram(int i)
{
    return diagrams.at(i);
}

int EModel::diagramCount()
{
    return diagrams.size();
}
