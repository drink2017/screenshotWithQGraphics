#include "myrectitem.h"
#include "commandmanager.h"
#include "screenshotview.h"

//----------------------------
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"
//----------------------------

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
        ellipseHandle->setPen(QPen(Qt::black));
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

    if((handles[0].contains(event->pos()) && ellipseHandles[0]->isVisible()) || (handles[4].contains(event->pos()) && ellipseHandles[4]->isVisible())){
        setCursor(Qt::SizeFDiagCursor);
    }else if((handles[1].contains(event->pos()) && ellipseHandles[1]->isVisible()) || (handles[5].contains(event->pos()) && ellipseHandles[5]->isVisible())){
        setCursor(Qt::SizeVerCursor);
    }else if((handles[2].contains(event->pos()) && ellipseHandles[2]->isVisible()) || (handles[6].contains(event->pos()) && ellipseHandles[6]->isVisible())){
        setCursor(Qt::SizeBDiagCursor);
    }else if((handles[3].contains(event->pos()) && ellipseHandles[3]->isVisible()) || (handles[7].contains(event->pos()) && ellipseHandles[7]->isVisible())){
        setCursor(Qt::SizeHorCursor);
    }else if(isMouseOnBoundary(event->pos())){
        setCursor(Qt::SizeAllCursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
}

QPainterPath myRectItem::getShadowPath(QPoint selectStart,QPoint selectEnd){
    QPainterPath globalPath;
    globalPath.lineTo(QGuiApplication::primaryScreen()->geometry().width(),0);
    globalPath.lineTo(QGuiApplication::primaryScreen()->geometry().width(),QGuiApplication::primaryScreen()->geometry().height());
    globalPath.lineTo(0,QGuiApplication::primaryScreen()->geometry().height());
    globalPath.closeSubpath();
    QPainterPath shadowPath;
    shadowPath.moveTo(selectStart);
    shadowPath.lineTo(selectEnd.x(),selectStart.y());
    shadowPath.lineTo(selectEnd);
    shadowPath.lineTo(selectStart.x(),selectEnd.y());
    shadowPath.closeSubpath();
    return globalPath.subtracted(shadowPath);
}


void myRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(isMouseOnBoundary(event->pos())){
        QGraphicsRectItem::mousePressEvent(event);
        commandManager::getInstance()->setEditingItem(true);
        type = mousePointIn(event->pos());
        before = this->rect();
        if(type == rect_no){
            setCursor(Qt::SizeAllCursor);
        }

        //-------------------------------
        myRectItem* moveBeforeItem = new myRectItem(this->rect());
        moveBeforeItem->setPos(this->pos());
        moveBeforeItem->setPen(this->pen());
        moveBeforeItem->setFlags(this->flags());
        order* moveOrder = new order();
        moveOrder->addToDeleteItem(moveBeforeItem);
        undoManager::getInstance()->pushOrder(moveOrder);
        //---------------------------------
    }else{
        QList<QGraphicsItem*> selectedItems = screenshotView::getInstance()->getScene()->selectedItems();
        if (!selectedItems.isEmpty()) {
            foreach (QGraphicsItem* item, selectedItems) {
                item->setSelected(false);
            }
        }

        type = mousePointIn(event->pos());
    }
}

void myRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->isSelected() && commandManager::getInstance()->isEditingItem() && type == rect_no){
        QGraphicsRectItem::mouseMoveEvent(event);
    }
    if(type == rect_top_left){
        setCursor(Qt::SizeFDiagCursor);
        QRectF now;
        if(event->pos().x() < before.bottomRight().x() && event->pos().y() < before.bottomRight().y()){
            now.setTopLeft(event->pos());
            now.setBottomRight(before.bottomRight());
        }else if(event->pos().x() < before.bottomRight().x()){
            now.setTopRight(before.bottomRight());
            now.setBottomLeft(event->pos());
        }else if(event->pos().y() < before.bottomRight().y()){
            now.setBottomLeft(before.bottomRight());
            now.setTopRight(event->pos());
        }else{
            now.setTopLeft(before.bottomRight());
            now.setBottomRight(event->pos());
        }
        this->setRect(now);
        updateEllipseHandles();
    }else if(type == rect_top){
        setCursor(Qt::SizeVerCursor);
        QRectF now;
        if(event->pos().y() < before.bottom()){
            now.setBottomLeft(before.bottomLeft());
            now.setTopRight(QPoint(before.right(),event->pos().y()));
        }else{
            now.setTopLeft(before.bottomLeft());
            now.setBottomRight(QPoint(before.right(),event->pos().y()));
        }
        setRect(now);
        updateEllipseHandles();
    }else if(type == rect_top_right){
        setCursor(Qt::SizeBDiagCursor);
        QRectF now;
        if(event->pos().x() < before.bottomLeft().x() && event->pos().y() < before.bottomLeft().y()){
            now.setTopLeft(event->pos());
            now.setBottomRight(before.bottomLeft());
        }else if(event->pos().x() < before.bottomLeft().x()){
            now.setBottomLeft(event->pos());
            now.setTopRight(before.bottomLeft());
        }else if(event->pos().y() < before.bottomLeft().y()){
            now.setBottomLeft(before.bottomLeft());
            now.setTopRight(event->pos());
        }else{
            now.setTopLeft(before.bottomLeft());
            now.setBottomRight(event->pos());
        }
        setRect(now);
        updateEllipseHandles();
    }else if(type == rect_right){
        setCursor(Qt::SizeHorCursor);
        QRectF now;
        if(event->pos().x() < before.left()){
            now.setTopLeft(QPoint(event->pos().x(),before.top()));
            now.setBottomRight(before.bottomLeft());
        }else{
            now.setTopLeft(before.topLeft());
            now.setBottomRight(QPoint(event->pos().x(),before.bottom()));
        }
        setRect(now);
        updateEllipseHandles();
    }else if(type == rect_bottom_right){
        setCursor(Qt::SizeFDiagCursor);
        QRectF now;
        if(event->pos().x() < before.topLeft().x() && event->pos().y() < before.topLeft().y()){
            now.setTopLeft(event->pos());
            now.setBottomRight(before.topLeft());
        }else if(event->pos().x() < before.topLeft().x()){
            now.setBottomLeft(event->pos());
            now.setTopRight(before.topLeft());
        }else if(event->pos().y() < before.topRight().y()){
            now.setBottomLeft(before.topLeft());
            now.setTopRight(event->pos());
        }else{
            now.setTopLeft(before.topLeft());
            now.setBottomRight(event->pos());
        }
        setRect(now);
        updateEllipseHandles();
    }else if(type == rect_bottom){
        setCursor(Qt::SizeVerCursor);
        QRectF now;
        if(event->pos().y() < before.top()){
            now.setTopLeft(QPoint(before.left(),event->pos().y()));
            now.setBottomRight(before.topRight());
        }else{
            now.setTopLeft(before.topLeft());
            now.setBottomRight(QPoint(before.right(),event->pos().y()));
        }
        setRect(now);
        updateEllipseHandles();
    }else if(type == rect_bottom_left){
        setCursor(Qt::SizeBDiagCursor);
        QRectF now;
        if(event->pos().x() < before.topRight().x() && event->pos().y() < before.topRight().y()){
            now.setTopLeft(event->pos());
            now.setBottomRight(before.topRight());
        }else if(event->pos().x() < before.topRight().x()){
            now.setBottomLeft(event->pos());
            now.setTopRight(before.topRight());
        }else if(event->pos().y() < before.topRight().y()){
            now.setBottomLeft(before.topRight());
            now.setTopRight(event->pos());
        }else{
            now.setTopLeft(before.topRight());
            now.setBottomRight(event->pos());
        }
        setRect(now);
        updateEllipseHandles();
    }else if(type == rect_left){
        setCursor(Qt::SizeHorCursor);
        QRectF now;
        if(event->pos().x() < before.right()){
            now.setTopLeft(QPoint(event->pos().x(),before.top()));
            now.setBottomRight(before.bottomRight());
        }else{
            now.setTopLeft(before.topRight());
            now.setBottomRight(QPoint(event->pos().x(),before.bottom()));
        }
        setRect(now);
        updateEllipseHandles();
    }
}

void myRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    if(this->isSelected()){
        QGraphicsRectItem::mouseReleaseEvent(event);
        commandManager::getInstance()->setEditingItem(false);

        //--------------------------------
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
        //--------------------------------
    }
}

bool myRectItem::isMouseOnBoundary(const QPointF &pos) const{
    if((pos.y() >= this->rect().y()-5 && pos.y() <= this->rect().y()+5 && pos.x() >= this->rect().x()-5 && pos.x() <= this->rect().right()+5) ||
       (pos.x() >= this->rect().x()-5 && pos.x() <= this->rect().x()+5 && pos.y() >= this->rect().y()-5 && pos.y() <= this->rect().bottom()+5) ||
       (pos.y() >= this->rect().bottom()-5 && pos.y() <= this->rect().bottom()+5 && pos.x() >= this->rect().x()-5 && pos.x() <= this->rect().right()+5) ||
       (pos.x() >= this->rect().right()-5 && pos.x() <= this->rect().right()+5 && pos.y() >= this->rect().y()-5 && pos.y() <= this->rect().bottom()+5)
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































