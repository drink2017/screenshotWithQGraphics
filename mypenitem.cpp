#include "mypenitem.h"
#include "commandmanager.h"
#include "screenshotview.h"

#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QGraphicsScene>
#include <QDebug>

myPenItem::myPenItem(QPainterPath path,QGraphicsItem* parent):QGraphicsPathItem(path,parent)
{
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

bool myPenItem::isMouseOnBoundary(const QPointF &pos) const{
    QPainterPathStroker stroker;
    stroker.setWidth(4);
    QPainterPath boundaryPath = stroker.createStroke(path());
    if(boundaryPath.contains(pos)){
        return true;
    }else{
        return false;
    }
}

void myPenItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(isMouseOnBoundary(event->pos())){
        QGraphicsPathItem::mousePressEvent(event);
        commandManager::getInstance()->setEditingItem(true);
    }else{
        scene()->clearSelection();
    }
}

void myPenItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->isSelected()){
        QGraphicsPathItem::mouseMoveEvent(event);
        QRectF sceneRect = this->mapToScene(this->boundingRect()).boundingRect();
        QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
        if(sceneRect.x() < selectRect.x() && sceneRect.y() < selectRect.y()){
            QPainterPath path = this->path();
            path.translate(selectRect.left()-sceneRect.left(),selectRect.top()-sceneRect.top());
            this->setPath(path);
        }else if(sceneRect.x() < selectRect.x() && sceneRect.bottom() > selectRect.bottom()){
            QPainterPath path = this->path();
            path.translate(selectRect.x()-sceneRect.x(),selectRect.bottom()-sceneRect.bottom());
            this->setPath(path);
        }else if(sceneRect.right() > selectRect.right() && sceneRect.top() < selectRect.top()){
            QPainterPath path = this->path();
            path.translate(selectRect.right()-sceneRect.right(),selectRect.top()-sceneRect.top());
            this->setPath(path);
        }else if(sceneRect.right() > selectRect.right() && sceneRect.bottom() > selectRect.bottom()){
            QPainterPath path = this->path();
            path.translate(selectRect.right()-sceneRect.right(),selectRect.bottom()-sceneRect.bottom());
            this->setPath(path);
        }else if(sceneRect.left() < selectRect.left()){
            QPainterPath path = this->path();
            path.translate(selectRect.left() - sceneRect.left(),0);
            this->setPath(path);
        }else if(sceneRect.top() < selectRect.top()){
            QPainterPath path = this->path();
            path.translate(0,selectRect.top()-sceneRect.top());
            this->setPath(path);
        }else if(sceneRect.right() > selectRect.right()){
            QPainterPath path = this->path();
            path.translate(selectRect.right()-sceneRect.right(),0);
            this->setPath(path);
        }else if(sceneRect.bottom() > selectRect.bottom()){
            QPainterPath path = this->path();
            path.translate(0,selectRect.bottom()-sceneRect.bottom());
            this->setPath(path);
        }
    }
}

void myPenItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->isSelected()){
        QGraphicsPathItem::mouseReleaseEvent(event);
        commandManager::getInstance()->setEditingItem(false);
    }
}

void myPenItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(isMouseOnBoundary(event->pos())){
        setCursor(Qt::SizeAllCursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
}
