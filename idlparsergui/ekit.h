#ifndef EKIT_H
#define EKIT_H
#include <QString>
#include <emodel.h>
#include "eobject.h"

class EKit : public EObject
{
    QString idlVersion;
    QString title;
    QString creationDate;
    QString projectName;
    EModel *model;
public:
    EKit();
    void setIdlVersion(QString idlVersion);
    QString getIdlVersion();
    void setTitle(QString title);
    QString getTitle();
    void setCreationDate(QString creationDate);
    QString getCreationDate();
    void setProjectName(QString projectName);
    QString getProjectName();
    void setModel(EModel *model);
    EModel *getModel();
};

#endif // EKIT_H
