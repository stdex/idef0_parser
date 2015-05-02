#include "ekit.h"

EKit::EKit()
    :EObject()
{
}
void EKit::setIdlVersion(QString idlVersion)
{
    this->idlVersion = idlVersion;
}

QString EKit::getIdlVersion()
{
    return idlVersion;
}

void EKit::setTitle(QString title)
{
    this->title = title;
}

QString EKit::getTitle()
{
    return title;
}

void EKit::setCreationDate(QString creationDate)
{
    this->creationDate = creationDate;
}

QString EKit::getCreationDate()
{
    return creationDate;
}

void EKit::setProjectName(QString projectName)
{
    this->projectName = projectName;
}

QString EKit::getProjectName()
{
    return projectName;
}

void EKit::setModel(EModel *model)
{
    this->model = model;
}

EModel *EKit::getModel()
{
    return model;
}
