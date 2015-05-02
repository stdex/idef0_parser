#ifndef ENOTE_H
#define ENOTE_H
#include <QString>
#include <QPointF>
#include "eobject.h"

class ENote : public EObject
{
    QString id;
    QPointF coord;
    QString text;
public:
    ENote();
    void setId(QString id);
    QString getId();
    void setCoord(QPointF coord);
    QPointF getCoord();
    void setText(QString text);
    QString getText();

};

#endif // ENOTE_H
