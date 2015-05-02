 /****************************************************************************
 **
 ** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include "graphwidget.h"
 #include "gedge.h"
 #include "node.h"

 #include <QtGui>

 #include <math.h>
 #include <QVector>
 #include <cmath>
 #include "graphdialog.h"

 using namespace std;

 GraphWidget::GraphWidget(QWidget *parent)
     : QGraphicsView(parent), timerId(0)
 {


     main_scene = new QGraphicsScene(this);
     main_scene->setSceneRect(-2000, -2000, 4000, 4000);
     main_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
     setScene(main_scene);
     setCacheMode(CacheNone);
     setViewportUpdateMode(BoundingRectViewportUpdate);
     setRenderHint(QPainter::Antialiasing);
     setTransformationAnchor(AnchorUnderMouse);
     scale(qreal(0.8), qreal(0.8));

 }
 void GraphWidget::itemMoved()
 {
     if (!timerId)
         timerId = startTimer(1000 / 25);
 }


 void GraphWidget::timerEvent(QTimerEvent *event)
 {
     Q_UNUSED(event);

     QList<Node *> nodes;
     foreach (QGraphicsItem *item, scene()->items()) {
         if (Node *node = qgraphicsitem_cast<Node *>(item))
             nodes << node;
     }

     foreach (Node *node, nodes)
         node->calculateForces();

     bool itemsMoved = false;
     foreach (Node *node, nodes) {
         if (node->advance())
             itemsMoved = true;
     }

     if (!itemsMoved) {
         killTimer(timerId);
         timerId = 0;
     }
 }

 void GraphWidget::wheelEvent(QWheelEvent *event)
 {
     scaleView(pow((double)2, event->delta() / 240.0));
 }

 void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
 {
 }

 void GraphWidget::scaleView(qreal scaleFactor)
 {
     qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
     if (factor < 0.07 || factor > 100)
         return;

     scale(scaleFactor, scaleFactor);
 }

 void GraphWidget::shuffle()
 {
     foreach (QGraphicsItem *item, scene()->items()) {
         if (qgraphicsitem_cast<Node *>(item))
             item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
     }
 }

 void GraphWidget::zoomIn()
 {
     scaleView(qreal(1.2));
 }

 void GraphWidget::zoomOut()
 {
     scaleView(1 / qreal(1.2));
 }

 void GraphWidget::updateTree(int nDiag,int nType)
 {
     QString filename = qApp->applicationDirPath() +"/file" + QString::number(nDiag) + QString::number(nType)+".xml";
     QFile file(filename);
     //Add nodes
     Node *varray[100];
     int garr[100];
     int i = 0;
     main_scene->clear();
     if(file.open(QIODevice::ReadOnly))
     {
         QString in;
         QTextStream stdIn(&file);
         in = stdIn.readAll();
         QByteArray arr = in.toLatin1();
         QBuffer buffer;
         QXmlStreamReader scan_Xml;	//получить QByteArray

         buffer.setBuffer(&arr);
         if(buffer.open(QIODevice::ReadOnly))
         {
             scan_Xml.setDevice(&buffer);
             scan_Xml.readNextStartElement();
             if(scan_Xml.isStartElement() && scan_Xml.name() == "top")
             {
                 scan_Xml.readNextStartElement();
                 if (scan_Xml.name() == "edges")
                 {
                     ((GraphDialog*)parent())->setWindowTitle("Diagram: " + scan_Xml.attributes().value("name").toString());
                     while (scan_Xml.readNextStartElement())
                     {
                         if (scan_Xml.name() == "edge")
                         {
                             int id = -1;
                             QString title;
                             int gotob = -1;
                             scan_Xml.readNextStartElement();
                             if (scan_Xml.name() == "id")
                             {
                                 scan_Xml.readNext();
                                 id = scan_Xml.text().toString().toInt();
                             }
                             scan_Xml.skipCurrentElement();
                             scan_Xml.readNextStartElement();
                             if (scan_Xml.name() == "title")
                             {
                                 scan_Xml.readNext();
                                 title = scan_Xml.text().toString();
                             }
                             scan_Xml.skipCurrentElement();
                             scan_Xml.readNextStartElement();
                             if (scan_Xml.name() == "goto")
                             {
                                 scan_Xml.readNext();
                                 QString strg = scan_Xml.text().toString();
                                 if(strg!="END")gotob = scan_Xml.text().toString().toInt()-1;
                                 else gotob = -1;
                             }
                             Node *node = new Node(this,id,title);
                             varray[i] = node;
                             garr[i] = gotob;
                             main_scene->addItem(varray[i]);
                             scan_Xml.skipCurrentElement();
                             scan_Xml.skipCurrentElement();
                             i++;
                         }
                     }
                 }
             }
         }
         file.close();
         //
        int num = i;
        Node *n1 = NULL;
        Node *n2 = NULL;
        for(int i = 0; i< num; i++)
        {
            n1 = varray[i];
            if(garr[i]!=-1)
            {
                n2 = varray[garr[i]];
                main_scene->addItem(new GEdge(n1, n2,""));
            }
            varray[i]->setPos(100*i,0);
        }
     }

 }


