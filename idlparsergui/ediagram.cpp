#include "ediagram.h"

EDiagram::EDiagram()
    : EObject()
{
}

void EDiagram::setDiagramType(QString diagramType)
{
    this->diagramType = diagramType;
}

QString EDiagram::getDiagramType()
{
    return diagramType;
}

void EDiagram::setCreationDate(QString creationDate)
{
    this->creationDate = creationDate;
}

QString EDiagram::getCreationDate()
{
    return creationDate;
}

void EDiagram::setRevisionDate(QString revisionDate)
{
    this->revisionDate = revisionDate;
}

QString EDiagram::getRevisionDate()
{
    return revisionDate;
}

void EDiagram::setTitle(QString title)
{
    this->title = title;
}

QString EDiagram::getTitle()
{
    return title;
}

void EDiagram::setStatus(QString status)
{
    this->status = status;
}

QString EDiagram::getStstus()
{
    return status;
}

void EDiagram::setBoxes(QList<EBox*> boxes)
{
    this->boxes = boxes;
}

QList<EBox*> EDiagram::getBoxes()
{
    return boxes;
}

void EDiagram::addBox(EBox *box)
{
    boxes<<box;
}

EBox *EDiagram::getBox(int i)
{
    return boxes.at(i);
}

int EDiagram::boxCount()
{
    return boxes.size();
}

void EDiagram::setArrowsegs(QList<EArrowSeg*> arrowsegs)
{
    this->arrowsegs = arrowsegs;
}

QList<EArrowSeg*> EDiagram::getArrowsegs()
{
    return arrowsegs;
}

void EDiagram::addArrowseg(EArrowSeg *arrowseg)
{
    arrowsegs<<arrowseg;
}

EArrowSeg *EDiagram::getArrowseg(int i)
{
    return arrowsegs.at(i);
}

int EDiagram::arrowsegCount()
{
    return arrowsegs.size();
}

void EDiagram::setNotes(QList<ENote*> notes)
{
    this->notes = notes;
}

QList<ENote*> EDiagram::getNotes()
{
    return notes;
}

void EDiagram::addNote(ENote *note)
{
    notes<<note;
}

ENote *EDiagram::getNote(int i)
{
    return notes.at(i);
}

int EDiagram::noteCount()
{
    return notes.size();
}
