#ifndef EBOX_H
#define EBOX_H
#include <QString>
#include <QPointF>
#include "eobject.h"
#include <QList>
#include <edge.h>

class EBox : public EObject
{
    QString id;
    QString name;
    QPointF coordX;
    QPointF coordY;
    QString details;
public:
    EBox();
    void setId(QString id);
    QString getId();
    void setName(QString name);
    QString getName();
    void setCoordX(QPointF coordX);
    QPointF getCoordX();
    void setCoordY(QPointF coordY);
    QPointF getCoordY();
    void setDetails(QString details);
    QString getDetails();
};

#endif // EBOX_H
