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

 #include <QGraphicsScene>
 #include <QGraphicsSceneMouseEvent>
 #include <QPainter>
 #include <QStyleOption>

 #include "gedge.h"
 #include "node.h"
 #include "graphwidget.h"

 Node::Node(GraphWidget *graphWidget)
     : graph(graphWidget)
 {
     setFlag(ItemIsMovable);
     setFlag(ItemSendsGeometryChanges);
     setCacheMode(DeviceCoordinateCache);
     setZValue(-1);
     this->nodeName = "";
     this->numberGroup = 0;
     this->isSelected = false;
     this->id = 0;
 }

 Node::Node(GraphWidget *graphWidget, QString name, int numberGroup)
         : graph(graphWidget)
 {
     setFlag(ItemIsMovable);
     setFlag(ItemSendsGeometryChanges);
     setCacheMode(DeviceCoordinateCache);
     setZValue(-1);
     this->nodeName = name;
     this->numberGroup = numberGroup;
     this->isSelected = false;
     this->id = 0;
 }
 Node::Node(GraphWidget *graphWidget, int id,  QString name, int numberGroup)
         : graph(graphWidget)
 {
     setFlag(ItemIsMovable);
     setFlag(ItemSendsGeometryChanges);
     setCacheMode(DeviceCoordinateCache);
     setZValue(-1);
     this->nodeName = name;
     this->numberGroup = numberGroup;
     this->isSelected = false;
     this->id = id;
 }

 void Node::addEdge(GEdge *edge)
 {
     edgeList << edge;
     edge->adjust();
 }

 QList<GEdge *> Node::edges() const
 {
     return edgeList;
 }

 void Node::calculateForces()
 {
     if (!scene() || scene()->mouseGrabberItem() == this) {
         newPos = pos();
         return;
     }

     // Sum up all forces pushing this item away
     qreal xvel = 0;
     qreal yvel = 0;
     foreach (QGraphicsItem *item, scene()->items()) {
         Node *node = qgraphicsitem_cast<Node *>(item);
         if (!node)
             continue;

         QPointF vec = mapToItem(node, 0, 0);
         qreal dx = vec.x();
         qreal dy = vec.y();
         double l = 2.0 * (dx * dx + dy * dy);
         if (l > 0) {
             xvel += (dx * 150.0) / l;
             yvel += (dy * 150.0) / l;
         }
     }

     // Now subtract all forces pulling items together
     double weight = (edgeList.size() + 1) * 15;
     foreach (GEdge *edge, edgeList) {
         QPointF vec;
         if (edge->sourceNode() == this)
             vec = mapToItem(edge->destNode(), 0, 0);
         else
             vec = mapToItem(edge->sourceNode(), 0, 0);
         xvel -= vec.x() / weight;
         yvel -= vec.y() / weight;
     }

     if (qAbs(xvel) < 2.01 && qAbs(yvel) < 2.01)
         xvel = yvel = 0;

     QRectF sceneRect = scene()->sceneRect();
     newPos = pos() + QPointF(xvel, yvel);
     newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
     newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
 }

 bool Node::advance()
 {
     if (newPos == pos())
         return false;

     setPos(newPos);
     return true;
 }

 QRectF Node::boundingRect() const
 {
     /*qreal adjust = 4;
     return QRectF( -10 - adjust, -10 - adjust,
                   46 + adjust, 46 + adjust);*/
     qreal adjust = 2;
 /*    return QRectF( -20 - adjust, -20 - adjust,
                   43 + adjust, 43 + adjust);*/
     return QRectF( -30 - adjust, -30 - adjust,
                   63 + adjust, 63 + adjust);


 }

 QPainterPath Node::shape() const
 {
     QPainterPath path;
     path.addEllipse(-20, -20, 40, 40);
     //path.addEllipse(-10, -10, 20, 20);
     return path;
 }

 void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
 {
     painter->setPen(QPen(QColor(0,0,0), 0));
     QBrush whitebrush(QColor(255,255,255));
     painter->setBrush(whitebrush);
     if(isSelected==true)painter->drawEllipse(-30, -30, 60, 60);

     QBrush brush(QColor(255,255,255));
     painter->setBrush(brush);

         painter->setPen(QPen(QColor(0,0,0), 0));
         painter->drawEllipse(-20, -20, 40, 40);
      if(this->nodeName.length()!=0)
      {
          painter->drawText(0-5*(nodeName.length()-1)/2,30,nodeName);
      }
 }

 QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     switch (change) {
     case ItemPositionHasChanged:
         foreach (GEdge *edge, edgeList)
             edge->adjust();
         graph->itemMoved();
         break;
     default:
         break;
     };

     return QGraphicsItem::itemChange(change, value);
 }

 void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }

 void Node::setSelected(bool selected)
 {
     this->isSelected = selected;
 }

 QString Node::getNodeName()
 {
     return nodeName;
 }

 int Node::getId()
 {
  return this->id;
 }
