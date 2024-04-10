#include "myrectitem.h"
#include "commandmanager.h"
#include "screenshotview.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

#include <QBrush>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QApplication>
#include <QDebug>
#include <QScreen>

myRectItem::myRectItem(QGraphicsItem* parent) : QGraphicsRectItem(parent)
{

}

myRectItem::myRectItem(const QRectF& rect, QGraphicsItem* parent):QGraphicsRectItem(rect,parent){
    setAcceptHoverEvents(true);

    createEllipseHandles();
    hideEllipseHandles();

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void myRectItem::createEllipseHandles(){
    for(int i = 0;i < 8;i++){
        QGraphicsEllipseItem* ellipseHandle = new QGraphicsEllipseItem(0,0,10,10,this);
        ellipseHandle->setBrush(QBrush(Qt::white));
        ellipseHandle->setPen(QPen(Qt::red));
        ellipseHandles.append(ellipseHandle);
    }

    updateEllipseHandles();
}

void myRectItem::updateEllipseHandles(){
    QRectF rect = this->rect();
    QList<QRectF> handles = {
        QRectF(rect.left()-5,rect.top()-5,10,10),
        QRectF((rect.left()+rect.right())/2-5,rect.top()-5,10,10),
        QRectF(rect.right()-5,rect.top()-5,10,10),
        QRectF(rect.right()-5,(rect.top()+rect.bottom())/2-5,10,10),
        QRectF(rect.right()-5,rect.bottom()-5,10,10),
        QRectF((rect.left()+rect.right())/2-5,rect.bottom()-5,10,10),
        QRectF(rect.left()-5,rect.bottom()-5,10,10),
        QRectF(rect.left()-5,(rect.top()+rect.bottom())/2-5,10,10)
    };

    for(int i = 0;i < 8;i++){
        ellipseHandles[i]->setRect(handles[i]);
    }
}

void myRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(option->state & QStyle::State_Selected){
        showEllipseHandles();
    }else{
        if(!ellipseHandles.isEmpty()){
            hideEllipseHandles();
        }
    }
    QGraphicsRectItem::paint(painter,option,widget);
}

void myRectItem::showEllipseHandles(){
    for(int i = 0;i < 8;i++){
        ellipseHandles[i]->show();
    }
}

void myRectItem::hideEllipseHandles(){
    for(int i = 0;i < 8;i++){
        ellipseHandles[i]->hide();
    }
}

void myRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if((mousePointIn(event->pos()) == rect_top_left && ellipseHandles[0]->isVisible()) || (mousePointIn(event->pos()) == rect_bottom_right && ellipseHandles[4]->isVisible())){
        setCursor(Qt::SizeFDiagCursor);
    }else if((mousePointIn(event->pos()) == rect_top && ellipseHandles[1]->isVisible()) || (mousePointIn(event->pos()) == rect_bottom && ellipseHandles[5]->isVisible())){
        setCursor(Qt::SizeVerCursor);
    }else if((mousePointIn(event->pos()) == rect_top_right && ellipseHandles[2]->isVisible()) || (mousePointIn(event->pos()) == rect_bottom_left && ellipseHandles[6]->isVisible())){
        setCursor(Qt::SizeBDiagCursor);
    }else if((mousePointIn(event->pos()) == rect_right && ellipseHandles[3]->isVisible()) || (mousePointIn(event->pos()) == rect_left && ellipseHandles[7]->isVisible())){
        setCursor(Qt::SizeHorCursor);
    }else if(isMouseOnBoundary(event->pos())){
        setCursor(Qt::SizeAllCursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
}

void myRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(isMouseOnBoundary(event->pos())){
        QGraphicsRectItem::mousePressEvent(event);
        commandManager::getInstance()->setEditingItem(true);
        type = mousePointIn(event->pos());
        before = this->rect();

        myRectItem* moveBeforeItem = new myRectItem(this->rect());
        moveBeforeItem->setPos(this->pos());
        moveBeforeItem->setPen(this->pen());
        moveBeforeItem->setFlags(this->flags());
        order* moveOrder = new order();
        moveOrder->addToDeleteItem(moveBeforeItem);
        undoManager::getInstance()->pushOrder(moveOrder);
    }else{
        this->scene()->clearSelection();
        type = mousePointIn(event->pos());
    }
}

void myRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF selectRect = mapFromScene(QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd())).boundingRect();
    if(this->isSelected() && type == rect_no){
        QGraphicsRectItem::mouseMoveEvent(event);
        QRectF sceneRect = this->mapToScene(this->rect()).boundingRect();
        QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd());
        if(sceneRect.left() < selectRect.left() && sceneRect.top() < selectRect.top()){
            QRectF rect = this->rect();
            rect.translate(selectRect.left()-sceneRect.left(),selectRect.top() - sceneRect.top());
            setRect(rect);
        }else if(sceneRect.left() < selectRect.left() && sceneRect.bottom() > selectRect.bottom()){
            QRectF rect = this->rect();
            rect.translate(selectRect.left()-sceneRect.left(),selectRect.bottom()-sceneRect.bottom());
            setRect(rect);
        }else if(sceneRect.right() > selectRect.right() && sceneRect.top() < selectRect.top()){
            QRectF rect = this->rect();
            rect.translate(selectRect.right()-sceneRect.right(),selectRect.top()-sceneRect.top());
            setRect(rect);
        }else if(sceneRect.right() > selectRect.right() && sceneRect.bottom() > selectRect.bottom()){
            QRectF rect = this->rect();
            rect.translate(selectRect.right()-sceneRect.right(),selectRect.bottom() - sceneRect.bottom());
            setRect(rect);
        }else if(sceneRect.left() < selectRect.left()){
            QRectF rect = this->rect();
            rect.translate(selectRect.left()-sceneRect.left(),0);
            setRect(rect);
        }else if(sceneRect.top() < selectRect.top()){
            QRectF rect = this->rect();
            rect.translate(0,selectRect.top()-sceneRect.top());
            setRect(rect);
        }else if(sceneRect.right() > selectRect.right()){
            QRectF rect = this->rect();
            rect.translate(selectRect.right()-sceneRect.right(),0);
            setRect(rect);
        }else if(sceneRect.bottom() > selectRect.bottom()){
            QRectF rect = this->rect();
            rect.translate(0,selectRect.bottom()-sceneRect.bottom());
            setRect(rect);
        }
    }
    if(type == rect_top_left){
        setCursor(Qt::SizeFDiagCursor);
        QRectF now = QRectF(event->pos(),before.bottomRight()).normalized();
        this->setRect(now.intersected(selectRect));
    }else if(type == rect_top){
        setCursor(Qt::SizeVerCursor);
        QRectF now = QRectF(QPointF(before.left(),event->pos().y()),before.bottomRight()).normalized();
        setRect(now.intersected(selectRect));
    }else if(type == rect_top_right){
        setCursor(Qt::SizeBDiagCursor);
        QRectF now = QRectF(event->pos(),before.bottomLeft()).normalized();
        setRect(now.intersected(selectRect));
    }else if(type == rect_right){
        setCursor(Qt::SizeHorCursor);
        QRectF now = QRectF(before.topLeft(),QPointF(event->pos().x(),before.bottom())).normalized();
        setRect(now.intersected(selectRect));
    }else if(type == rect_bottom_right){
        setCursor(Qt::SizeFDiagCursor);
        QRectF now = QRectF(before.topLeft(),event->pos()).normalized();
        setRect(now.intersected(selectRect));
    }else if(type == rect_bottom){
        setCursor(Qt::SizeVerCursor);
        QRectF now = QRectF(before.topLeft(),QPointF(before.right(),event->pos().y())).normalized();
        setRect(now.intersected(selectRect));
    }else if(type == rect_bottom_left){
        setCursor(Qt::SizeBDiagCursor);
        QRectF now = QRectF(event->pos(),before.topRight()).normalized();
        setRect(now.intersected(selectRect));
    }else if(type == rect_left){
        setCursor(Qt::SizeHorCursor);
        QRectF now = QRectF(QPointF(event->pos().x(),before.top()),before.bottomRight()).normalized();
        setRect(now.intersected(selectRect));
    }
    updateEllipseHandles();
}

void myRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    if(this->isSelected()){
        QGraphicsRectItem::mouseReleaseEvent(event);
        commandManager::getInstance()->setEditingItem(false);

        order* moveOrder = undoManager::getInstance()->popOrder();
        myRectItem* moveBeforeItem = dynamic_cast<myRectItem*>(moveOrder->getDeleteItem().back());
        if(moveBeforeItem->rect() == this->rect() && moveBeforeItem->pos() == this->pos()){
            QQueue<QGraphicsItem*> deleteItem = moveOrder->getDeleteItem();
            delete deleteItem.back();
            delete moveOrder;
        }else{
            moveOrder->addToAddItem(this);
            undoManager::getInstance()->pushOrder(moveOrder);
            redoManager::getInstance()->clear();
        }
    }
}

bool myRectItem::isMouseOnBoundary(const QPointF &pos) const{
    if((pos.y() <= this->rect().y()+5) ||
       (pos.x() <= this->rect().left()+5) ||
       (pos.x() >= this->rect().right()-5) ||
       (pos.y() >= this->rect().bottom()-5)
            ){
        return true;
    }else{
        return false;
    }
}

pointIn myRectItem::mousePointIn(QPointF pos){
    QRectF rect = this->rect();
    QList<QRectF> handles = {
        QRectF(rect.left()-5,rect.top()-5,10,10),
        QRectF((rect.left()+rect.right())/2-5,rect.top()-5,10,10),
        QRectF(rect.right()-5,rect.top()-5,10,10),
        QRectF(rect.right()-5,(rect.top()+rect.bottom())/2-5,10,10),
        QRectF(rect.right()-5,rect.bottom()-5,10,10),
        QRectF((rect.left()+rect.right())/2-5,rect.bottom()-5,10,10),
        QRectF(rect.left()-5,rect.bottom()-5,10,10),
        QRectF(rect.left()-5,(rect.top()+rect.bottom())/2-5,10,10)
    };

    if(handles[0].contains(pos) && ellipseHandles[0]->isVisible()){
        return rect_top_left;
    }else if(handles[1].contains(pos) && ellipseHandles[1]->isVisible()){
        return rect_top;
    }else if(handles[2].contains(pos) && ellipseHandles[2]->isVisible()){
        return rect_top_right;
    }else if(handles[3].contains(pos) && ellipseHandles[3]->isVisible()){
        return rect_right;
    }else if(handles[4].contains(pos) && ellipseHandles[4]->isVisible()){
        return rect_bottom_right;
    }else if(handles[5].contains(pos) && ellipseHandles[5]->isVisible()){
        return rect_bottom;
    }else if(handles[6].contains(pos) && ellipseHandles[6]->isVisible()){
        return rect_bottom_left;
    }else if(handles[7].contains(pos) && ellipseHandles[7]->isVisible()){
        return rect_left;
    }else{
        return rect_no;
    }
}
















