#ifndef EDIAGRAM_H
#define EDIAGRAM_H
#include <QString>
#include <QList>
#include "ebox.h"
#include "earrowseg.h"
#include "enote.h"
#include "eobject.h"

class EDiagram : public EObject
{
    QString diagramType;
    QString creationDate;
    QString revisionDate;
    QString title;
    QString status;
    QList<EBox*> boxes;
    QList<EArrowSeg*> arrowsegs;
    QList<ENote*> notes;
public:
    EDiagram();
    void setDiagramType(QString diagramType);
    QString getDiagramType();
    void setCreationDate(QString creationDate);
    QString getCreationDate();
    void setRevisionDate(QString revisionDate);
    QString getRevisionDate();
    void setTitle(QString title);
    QString getTitle();
    void setStatus(QString status);
    QString getStstus();

    void setBoxes(QList<EBox*> boxes);
    QList<EBox*> getBoxes();
    void addBox(EBox *box);
    EBox *getBox(int i);
    int boxCount();

    void setArrowsegs(QList<EArrowSeg*> arrowsegs);
    QList<EArrowSeg*> getArrowsegs();
    void addArrowseg(EArrowSeg *arrowseg);
    EArrowSeg *getArrowseg(int i);
    int arrowsegCount();

    void setNotes(QList<ENote*> notes);
    QList<ENote*> getNotes();
    void addNote(ENote *note);
    ENote *getNote(int i);
    int noteCount();
};

#endif // EDIAGRAM_H
