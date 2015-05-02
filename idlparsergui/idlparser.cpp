#include "idlparser.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QStringList>

IdlParser::IdlParser(QString filename)
{
    this->filename = filename;
    startKeys<<"KIT";
    endKeys<<"ENDKIT";
    startKeys<<"MODEL";
    endKeys<<"ENDMODEL";
    startKeys<<"DIAGRAM";
    endKeys<<"ENDDIAGRAM";
    startKeys<<"BOX";
    endKeys<<"ENDBOX";
    startKeys<<"ARROWSEG";
    endKeys<<"ENDSEG";
    startKeys<<"NOTE";
    endKeys<<"ENDNOTE";
    EKit *kit = new EKit();
    EModel *model = new EModel();
    EDiagram *diagram = new EDiagram();
    EBox *box = new EBox();
    EArrowSeg *seg = new EArrowSeg();
    ENote *note = new ENote();
    elements<<kit;
    elements<<model;
    elements<<diagram;
    elements<<box;
    elements<<seg;
    elements<<note;
    edges.clear();
}

void IdlParser::parse()
{
    QFile file(filename);
    file.open(QFile::ReadOnly | QFile::Text);
    int currentLiter = -1;

    QTextStream in(&file);
    QString allString = in.readAll();
    QStringList myList = allString.split("\n");
    for(int i=0; i<myList.size(); i++ )
    {
        QString line = myList.at(i);//gets line from file
        QStringList literals = line.trimmed().split(QRegExp("\\s"));
        //Level 1 literals
        if(literals.at(0).trimmed().compare(startKeys.at(0))==0)//kit
        {
            currentLiter = 0;
            EKit *kit = new EKit();
            elements.replace(0, kit);
        }
        else if(literals.at(0).trimmed().compare(startKeys.at(1))==0)//model
        {
            currentLiter = 1;
            EModel *model = new EModel();
            model->setName(clearName(line.trimmed().mid(5).replace(";"," ").trimmed()));
            elements.replace(1, model);
        }
        else if(literals.at(0).trimmed().compare(startKeys.at(2))==0)//diagram
        {
            currentLiter = 2;
            EDiagram *diagram = new EDiagram();
            diagram->setDiagramType(line.trimmed().mid(7).replace(";"," ").trimmed());
            elements.replace(2, diagram);
        }
        else if(literals.at(0).trimmed().compare(startKeys.at(3))==0)//box
        {
            if(currentLiter==3)
            {
                if((literals.at(0).trimmed().compare(startKeys.at(3))==0)&&(literals.at(1).trimmed().compare("COORDINATES")==0))
                {
                    int d1 = line.trimmed().indexOf("(")+1;
                    int d2 = line.trimmed().indexOf(";");
                    int d3 = line.trimmed().indexOf(")");
                    QString x1 = line.trimmed().mid(d1,d2-d1).replace(";"," ").replace(",",".");
                    QString y1 = line.trimmed().mid(d2+1,d3-d2-1).replace(";"," ").replace(",",".");
                    d1 = line.trimmed().indexOf("(",d1+1)+1;
                    d2 = line.trimmed().indexOf(";",d2+1);
                    d3 = line.trimmed().indexOf(")",d3+1);
                    QString x2 = line.trimmed().mid(d1,d2-d1).replace(";"," ").replace(",",".");
                    QString y2 = line.trimmed().mid(d2+1,d3-d2-1).replace(";"," ").replace(",",".");
                    ((EBox*)elements.at(3))->setCoordX(QPointF(x1.toDouble(),y1.toDouble()));
                    ((EBox*)elements.at(3))->setCoordY(QPointF(x2.toDouble(),y2.toDouble()));
                }
                continue;
            }
            currentLiter = 3;
            EBox *box = new EBox();
            box->setId(line.trimmed().mid(3).replace(";"," ").trimmed());
            elements.replace(3, box);

        }
        else if(literals.at(0).trimmed().compare(startKeys.at(4))==0)//arrowseg
        {
            currentLiter = 4;
            EArrowSeg *arrowseg = new EArrowSeg();
            arrowseg->setId(line.trimmed().mid(QString("ARROWSEG").length()+1).replace(";"," ").trimmed());
            elements.replace(4, arrowseg);
        }
        else if((currentLiter != 5)&&(literals.at(0).trimmed().compare(startKeys.at(5))==0))
        {
            currentLiter = 5;
            ENote *note = new ENote();
            note->setId(line.trimmed().mid(QString("NOTE").length()+1).replace(";"," ").trimmed());
            elements.replace(5, note);
        }
        else if(literals.at(0).trimmed().compare(endKeys.at(0))==0)
        {
            currentLiter = -1;
            ((EKit*)elements.at(0))->setModel((EModel*)elements.at(1));
            ekit = ((EKit*)elements.at(0));
        }
        else if(literals.at(0).trimmed().compare(endKeys.at(1))==0)
        {
            currentLiter = 0;
        }
        else if(literals.at(0).trimmed().compare(endKeys.at(2))==0)
        {
            currentLiter = 1;
            ((EModel*)elements.at(1))->addDiagram((EDiagram*)elements.at(2));

        }
        else if(literals.at(0).trimmed().compare(endKeys.at(3))==0)
        {
            currentLiter = 2;
            ((EDiagram*)elements.at(2))->addBox((EBox*)elements.at(3));
        }
        else if(literals.at(0).trimmed().compare(endKeys.at(4))==0)
        {
            currentLiter = 2;
            ((EDiagram*)elements.at(2))->addArrowseg((EArrowSeg*)elements.at(4));
            qDebug(endKeys.at(4).toLatin1());
        }
        else if(literals.at(0).trimmed().compare(endKeys.at(5))==0)
        {
            currentLiter = 2;
            ((EDiagram*)elements.at(2))->addNote((ENote*)elements.at(5));
            qDebug(endKeys.at(5).toLatin1());
        }
        else//Gets parameters
        {
            if(currentLiter==0)//KIT
            {
                if(literals.at(0).trimmed().compare("IDL")==0)
                {
                    ((EKit*)elements.at(0))->setIdlVersion(line.trimmed().mid(11).replace(";"," ").trimmed());
                }
                if(literals.at(0).trimmed().compare("TITLE")==0)
                {
                    ((EKit*)elements.at(0))->setTitle(clearName(line.trimmed().mid(5).replace(";"," ").trimmed()));
                }
                if(literals.at(0).trimmed().compare("CREATION")==0)
                {
                    ((EKit*)elements.at(0))->setCreationDate(line.trimmed().mid(13).replace(";"," ").trimmed());
                }
                if(literals.at(0).trimmed().compare("PROJECT")==0)
                {
                    ((EKit*)elements.at(0))->setProjectName(clearName(line.trimmed().mid(12).replace(";"," ").trimmed()));
                }
            }
            //
            if(currentLiter==2)//DIAGRAM
            {
                if(literals.at(0).trimmed().compare("CREATION")==0)
                {
                    ((EDiagram*)elements.at(2))->setCreationDate(line.trimmed().mid(QString("CREATION DATE").length()+1).replace(";"," ").trimmed());
                    //qDebug(line.trimmed().mid(QString("CREATION DATE").length()+1).replace(";"," ").trimmed().toLatin1());
                }
                if(literals.at(0).trimmed().compare("REVISION")==0)
                {
                    ((EDiagram*)elements.at(2))->setRevisionDate(line.trimmed().mid(QString("REVISION DATE").length()+1).replace(";"," ").trimmed());
                    //qDebug(line.trimmed().mid(QString("CREATION DATE").length()+1).replace(";"," ").trimmed().toLatin1());
                }
                if(literals.at(0).trimmed().compare("TITLE")==0)
                {
                    ((EDiagram*)elements.at(2))->setTitle(clearName(line.trimmed().mid(QString("TITLE").length()+1).replace(";"," ").trimmed()));
                    //qDebug(line.trimmed().mid(QString("CREATION DATE").length()+1).replace(";"," ").trimmed().toLatin1());
                }
                if(literals.at(0).trimmed().compare("STATUS")==0)
                {
                    ((EDiagram*)elements.at(2))->setStatus(line.trimmed().mid(QString("STATUS").length()+1).replace(";"," ").trimmed());
                    //qDebug(line.trimmed().mid(QString("CREATION DATE").length()+1).replace(";"," ").trimmed().toLatin1());
                }

            }
            if(currentLiter==3)//BOX
            {
                if(literals.at(0).trimmed().compare("NAME")==0)
                {
                    ((EBox*)elements.at(3))->setName(clearName(line.trimmed().mid(line.trimmed().indexOf("}")+1).replace(";"," ").replace("'"," ").trimmed()));
                    //qDebug(line.trimmed().mid(QString("CREATION DATE").length()+1).replace(";"," ").trimmed().toLatin1());
                }
                if(literals.at(0).trimmed().compare("DETAIL")==0)
                {
                    ((EBox*)elements.at(3))->setDetails(line.trimmed().mid(QString("DETAIL").length()+1).replace(";"," ").trimmed());
                    //qDebug(line.trimmed().mid(QString("CREATION DATE").length()+1).replace(";"," ").trimmed().toLatin1());
                }

            }
            if(currentLiter==4)//ARROWSEG
            {
                if(literals.at(0).trimmed().compare("SOURCE")==0)
                {
                    ((EArrowSeg*)elements.at(4))->setSource(line.trimmed().mid(QString("SOURCE").length()+1).replace(";"," ").trimmed());
                    //qDebug(line.trimmed().mid(QString("CREATION DATE").length()+1).replace(";"," ").trimmed().toLatin1());
                }
                if(literals.at(0).trimmed().compare("PATH")==0)
                {
                    int d1 = line.trimmed().indexOf("(")+1;
                    int d2 = line.trimmed().indexOf(";");
                    int d3 = line.trimmed().indexOf(")");
                    QString x1 = line.trimmed().mid(d1,d2-d1).replace(";"," ").replace(",",".");
                    QString y1 = line.trimmed().mid(d2+1,d3-d2-1).replace(";"," ").replace(",",".");
                    ((EArrowSeg*)elements.at(4))->addPath(QPointF(x1.toDouble(),y1.toDouble()));
                    while(d1!=-1)
                    {
                        d1 = line.trimmed().indexOf("(",d1+1)+1;
                        d2 = line.trimmed().indexOf(";",d2+1);
                        d3 = line.trimmed().indexOf(")",d3+1);
                        if(d3==-1)break;
                        x1 = line.trimmed().mid(d1,d2-d1).replace(";"," ").replace(",",".");
                        y1 = line.trimmed().mid(d2+1,d3-d2-1).replace(";"," ").replace(",",".");
                        ((EArrowSeg*)elements.at(4))->addPath(QPointF(x1.toDouble(),y1.toDouble()));

                    }
                }
                if((literals.at(0).trimmed().compare("LABEL")==0)&&(literals.at(1).trimmed().compare("COORDINATES")!=0))
                {
                    ((EArrowSeg*)elements.at(4))->setLabel(clearName(line.trimmed().mid(QString("LABEL '").length()+1).replace(";"," ").replace("'"," ").trimmed()));
                    //qDebug(line.trimmed().mid(QString("CREATION DATE").length()+1).replace(";"," ").trimmed().toLatin1());
                }
                if((literals.at(0).trimmed().compare("LABEL")==0)&&(literals.at(1).trimmed().compare("COORDINATES")==0))
                {
                    int d11 = line.trimmed().indexOf("(")+1;
                    int d12 = line.trimmed().indexOf(";");
                    int d13 = line.trimmed().indexOf(")");
                    QString x11 = line.trimmed().mid(d11,d12-d11).replace(";"," ").replace(",",".");
                    QString y11 = line.trimmed().mid(d12+1,d13-d12-1).replace(";"," ").replace(",",".");
                    ((EArrowSeg*)elements.at(4))->setLabelcoord(QPointF(x11.toDouble(),y11.toDouble()));
                }
                if(literals.at(0).trimmed().compare("SINK")==0)
                {
                    ((EArrowSeg*)elements.at(4))->setSink(line.trimmed().mid(QString("SINK").length()+1).replace(";"," ").trimmed());
                }
                if((literals.at(0).trimmed().compare("SQUIGGLE")==0)&&(literals.at(1).trimmed().compare("COORDINATES")==0))
                {
                    int d1 = line.trimmed().indexOf("(")+1;
                    int d2 = line.trimmed().indexOf(";");
                    int d3 = line.trimmed().indexOf(")");
                    QString x1 = line.trimmed().mid(d1,d2-d1).replace(";"," ").replace(",",".");
                    QString y1 = line.trimmed().mid(d2+1,d3-d2-1).replace(";"," ").replace(",",".");
                    d1 = line.trimmed().indexOf("(",d1+1)+1;
                    d2 = line.trimmed().indexOf(";",d2+1);
                    d3 = line.trimmed().indexOf(")",d3+1);
                    QString x2 = line.trimmed().mid(d1,d2-d1).replace(";"," ").replace(",",".");
                    QString y2 = line.trimmed().mid(d2+1,d3-d2-1).replace(";"," ").replace(",",".");
                    ((EArrowSeg*)elements.at(4))->setSquiggle_coordX(QPointF(x1.toDouble(),y1.toDouble()));
                    ((EArrowSeg*)elements.at(4))->setSquiggle_coordY(QPointF(x2.toDouble(),y2.toDouble()));
                }

            }
            if(currentLiter==5)//NOTE
            {
                if((literals.at(0).trimmed().compare("NOTE")==0)&&(literals.at(1).trimmed().compare("COORDINATES")==0))
                {
                    int d1 = line.trimmed().indexOf("(")+1;
                    int d2 = line.trimmed().indexOf(";");
                    int d3 = line.trimmed().indexOf(")");
                    QString x1 = line.trimmed().mid(d1,d2-d1).replace(";"," ").replace(",",".");
                    QString y1 = line.trimmed().mid(d2+1,d3-d2-1).replace(";"," ").replace(",",".");
                    ((ENote*)elements.at(5))->setCoord(QPointF(x1.toDouble(),y1.toDouble()));
                }
                if((literals.at(0).trimmed().compare("NOTE")==0)&&(literals.at(1).trimmed().compare("TEXT")==0))
                {
                    ((ENote*)elements.at(5))->setText(clearName(line.trimmed().mid(QString("NOTE TEXT '").length()+1).replace(";"," ").replace("'"," ").trimmed()));
                    //qDebug(line.trimmed().mid(QString("CREATION DATE").length()+1).replace(";"," ").trimmed().toLatin1());
                }

            }

        }
        if(line.indexOf("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0")!=-1)
        {
            //—начала выберем и распрасим весь список меток
            QString samplestring = "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
            int index = line.indexOf(samplestring);
            QString tmpLine = line.mid(index+QString(samplestring).length(),line.indexOf("M",index+1)-index-QString(samplestring).length()+1).replace("'"," ").trimmed();
            tmpLine = tmpLine.simplified();
            QStringList arr = tmpLine.split(QRegExp("\\s"));
            int numLabels = 0;
            if(arr.at(0)=="B")//list of labels
            {
                numLabels = arr.at(2).toInt();
                for(int lm=0; lm < numLabels; lm++)
                {
                    typeEdge *lineEdge = new typeEdge();
                    lineEdge->id = arr.at(4 + 7*lm).toInt();
                    lineEdge->name = arr.at(10 + 7*lm);
                    etypes<<lineEdge;
                }
                //ƒалее парсинг самих данных
                int indexData = 4 + 7*numLabels;
                int curIndex = 0;
                while(1)
                {
                    if(arr.at(indexData + curIndex*6)=="B")
                    {
                        indexData += 4;
                    }
                    if(arr.at(indexData + curIndex*6)=="M")
                    {
                        break;
                    }
                    edge *el = new edge();
                    el->setId(arr.at(indexData + curIndex*6+1));
                    el->setType(getTypeFromId(arr.at(indexData + curIndex*6+0).toInt()));
                    QString coords = arr.at(indexData + curIndex*6+5);
                    int sep = coords.indexOf("_");
                    el->setStart(coords.mid(0,sep).trimmed().toInt());
                    QList<int*> ends;
                    if(sep!=-1)
                    {
                        coords = coords.mid(sep+1);
                        QStringList arr2 = coords.split("|");
                        for(int lj=0; lj<arr2.size(); lj++)
                        {
                            int iend = arr2.at(lj).trimmed().toInt();
                            int *end = new int();
                            *end = iend;
                            ends<<end;
                        }
                    }
                    el->setEnds(ends);
                    edges<<el;
                    curIndex++;
                }
            }
        }
        //label1 parsing
       /* if(line.indexOf("\'label1\'")!=-1)
        {
           int index = line.indexOf("\'label1\'");
           QString tmpLine = line.mid(index+QString("\'label1\'").length(),line.indexOf("M",index+1)-index-QString("\'label1\'").length()).replace("'"," ").trimmed();
           tmpLine = tmpLine.simplified();
           QStringList arr = tmpLine.split(QRegExp("\\s"));
           for(int li = 0; li < arr.size()/10; li++)
           {
               if(arr.at(li*10)=="B")//box
               {
                    edge *el = new edge();
                    el->setId(arr.at(li*10+5));
                    QString coords = arr.at(li*10+9);
                    int sep = coords.indexOf("_");
                    el->setStart(coords.mid(0,sep).trimmed().toInt());
                    QList<int*> ends;
                    if(sep!=-1)
                    {
                        coords = coords.mid(sep+1);
                        QStringList arr2 = coords.split("|");
                        for(int lj=0; lj<arr2.size(); lj++)
                        {
                            int iend = arr2.at(lj).trimmed().toInt();
                            int *end = new int();
                            *end = iend;
                            ends<<end;
                        }
                    }
                    el->setEnds(ends);
                    edges<<el;
               }
           }
        }*/
    }
    file.close();
    int lk = 0;
    int jk = 0;
    foreach(edge *ed1, edges)
    {
        QString id = ed1->getId();
        jk = 0;
        foreach(EDiagram *diag, ekit->getModel()->getDiagrams())
        {
            foreach(EBox *box1, diag->getBoxes())
            {
                if(box1->getId()==id)
                {

                    edges.at(lk)->setName(box1->getName());
                    edges.at(lk)->setNDiag(jk);
                    break;
                }
            }
            jk++;
        }
        lk++;
    }

    return;
}

EKit *IdlParser::getEkit()
{
    return ekit;
}

QString IdlParser::clearName(QString param)
{
    QString tmpstr  = param.mid(param.indexOf("}")+1);
    tmpstr = tmpstr.replace("'"," ").replace("<CR>"," ").trimmed();
    return tmpstr;
}

QList<edge*> IdlParser::getEdges()
{
    return edges;
}

typeEdge *IdlParser::getTypeFromId(int id)
{
    foreach(typeEdge *te,etypes)
    {
        if(te->id==id)return te;
    }
    return NULL;
}
