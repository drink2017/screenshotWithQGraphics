#include "myellipseitem.h"
#include "commandmanager.h"
#include "screenshotview.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

#include <QBrush>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>
#include <QGraphicsScene>

myEllipseItem::myEllipseItem()
{

}

myEllipseItem::myEllipseItem(const QRectF& rect,QGraphicsItem* parent):QGraphicsEllipseItem(rect,parent){
    setAcceptHoverEvents(true);
    createEllipseHandles();
    hideEllipseHandles();

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}


void myEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(option->state & QStyle::State_Selected){
        showEllipseHandles();
    }else{
        if(!ellipseHandles.isEmpty()){
            hideEllipseHandles();
        }
    }
    QGraphicsEllipseItem::paint(painter,option,widget);
}

void myEllipseItem::createEllipseHandles(){
    for(int i = 0;i < 4;i++){
        QGraphicsEllipseItem* ellipseHandle = new QGraphicsEllipseItem(0,0,10,10,this);
        ellipseHandle->setBrush(QBrush(Qt::white));
        ellipseHandle->setPen(QPen(Qt::red));
        ellipseHandles.append(ellipseHandle);
    }

    updateEllipseHandles();
}

void myEllipseItem::showEllipseHandles(){
    for(int i = 0;i < 4;i++){
        ellipseHandles[i]->show();
    }
}

void myEllipseItem::hideEllipseHandles(){
    for(int i = 0;i < 4;i++){
        ellipseHandles[i]->hide();
    }
}

void myEllipseItem::updateEllipseHandles(){
    QRectF rect = this->rect();
    QList<QRectF> handles = {
        QRectF((rect.left()+rect.right())/2-5,rect.top()-5,10,10),
        QRectF(rect.right()-5,(rect.top()+rect.bottom())/2-5,10,10),
        QRectF((rect.left()+rect.right())/2-5,rect.bottom()-5,10,10),
        QRectF(rect.left()-5,(rect.top()+rect.bottom())/2-5,10,10)
    };

    for(int i = 0;i < 4;i++){
        ellipseHandles[i]->setRect(handles[i]);
    }
}

bool myEllipseItem::isMouseOnBoundary(const QPointF &pos) const{
    QPainterPath shapePath = this->shape(); // 获取项的形状

    qreal offset = 5;
    QPainterPath concentricEllipsePath;
    concentricEllipsePath.addEllipse(shapePath.boundingRect().adjusted(offset, offset, -offset, -offset));

    bool isOnEdge = (shapePath.subtracted(concentricEllipsePath)).contains(pos); // 检查鼠标位置是否在形状的边缘上
    return isOnEdge;
}

void myEllipseItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if((mousePointIn(event->pos()) == ellipse_rect_top && ellipseHandles[0]->isVisible()) || (mousePointIn(event->pos()) == ellipse_rect_bottom && ellipseHandles[2]->isVisible())){
        setCursor(Qt::SizeVerCursor);
    }else if((mousePointIn(event->pos()) == ellipse_rect_right && ellipseHandles[1]->isVisible()) || (mousePointIn(event->pos()) == ellipse_rect_left && ellipseHandles[3]->isVisible())){
        setCursor(Qt::SizeHorCursor);
    }else if(isMouseOnBoundary(event->pos())){
        setCursor(Qt::SizeAllCursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
}

void myEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(isMouseOnBoundary(event->pos())){
        QGraphicsEllipseItem::mousePressEvent(event);
        commandManager::getInstance()->setEditingItem(true);
        type = mousePointIn(event->pos());
        before = this->rect();

        myEllipseItem* moveBeforeItem = new myEllipseItem(this->rect());
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

void myEllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->isSelected() && commandManager::getInstance()->isEditingItem() && type == ellipse_rect_no){
        QGraphicsEllipseItem::mouseMoveEvent(event);
    }
    if(type == ellipse_rect_top){
        setCursor(Qt::SizeVerCursor);
        QRectF now = QRectF(QPointF(before.left(),event->pos().y()),before.bottomRight()).normalized();
        this->setRect(now);
        updateEllipseHandles();
    }else if(type == ellipse_rect_right){
        setCursor(Qt::SizeHorCursor);
        QRectF now = QRectF(before.topLeft(),QPointF(event->pos().x(),before.bottom())).normalized();
        this->setRect(now);
        updateEllipseHandles();
    }else if(type == ellipse_rect_bottom){
        setCursor(Qt::SizeVerCursor);
        QRectF now = QRectF(before.topLeft(),QPointF(before.right(),event->pos().y())).normalized();
        this->setRect(now);
        updateEllipseHandles();
    }else if(type == ellipse_rect_left){
        setCursor(Qt::SizeHorCursor);
        QRectF now = QRectF(QPointF(event->pos().x(),before.top()),before.bottomRight()).normalized();
        this->setRect(now);
        updateEllipseHandles();
    }
}

void myEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    if(this->isSelected()){
        QGraphicsEllipseItem::mouseReleaseEvent(event);        
        QRectF sceneRect = this->mapToScene(this->boundingRect()).boundingRect();
        QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd());
        if(sceneRect.left() < selectRect.x()){
            this->setX(selectRect.x() - this->rect().x());
            sceneRect.translate(selectRect.x() - sceneRect.x(),0);
            setNowRect(sceneRect);
        }
        if(sceneRect.top() < selectRect.top()){
            this->setY(selectRect.y() - this->rect().y());
            sceneRect.translate(0,selectRect.y() - sceneRect.top());
            setNowRect(sceneRect);
        }
        if(sceneRect.right() > selectRect.right()){
            this->setX(selectRect.right() - this->rect().right());
            sceneRect.translate(selectRect.right() - sceneRect.right(),0);
            setNowRect(sceneRect);
        }
        if(sceneRect.bottom() > selectRect.bottom()){
            this->setY(selectRect.bottom() - this->rect().bottom());
            sceneRect.translate(0,selectRect.bottom() - sceneRect.bottom());
            setNowRect(sceneRect);
        }
        if(sceneRect.left() > selectRect.x() &&
           sceneRect.top() > selectRect.top() &&
           sceneRect.right() < selectRect.right() &&
           sceneRect.bottom() < selectRect.bottom()){
           setNowRect(sceneRect);
        }
        commandManager::getInstance()->setEditingItem(false);

        order* moveOrder = undoManager::getInstance()->popOrder();
        myEllipseItem* moveBeforeItem = dynamic_cast<myEllipseItem*>(moveOrder->getDeleteItem().back());
        if(moveBeforeItem->rect() == this->rect() && moveBeforeItem->pos() == this->pos()){
            delete moveBeforeItem;
            delete moveOrder;
        }else{
            moveOrder->addToAddItem(this);
            undoManager::getInstance()->pushOrder(moveOrder);
            redoManager::getInstance()->clear();
        }
    }
}

pointInEllipse myEllipseItem::mousePointIn(QPointF pos){
    QRectF rect = this->rect();
    QList<QRectF> handles = {
        QRectF((rect.left()+rect.right())/2-5,rect.top()-5,10,10),
        QRectF(rect.right()-5,(rect.top()+rect.bottom())/2-5,10,10),
        QRectF((rect.left()+rect.right())/2-5,rect.bottom()-5,10,10),
        QRectF(rect.left()-5,(rect.top()+rect.bottom())/2-5,10,10)
    };

    if(handles[0].contains(pos) && ellipseHandles[0]->isVisible()){
        return ellipse_rect_top;
    }else if(handles[1].contains(pos) && ellipseHandles[1]->isVisible()){
        return ellipse_rect_right;
    }else if(handles[2].contains(pos) && ellipseHandles[2]->isVisible()){
        return ellipse_rect_bottom;
    }else if(handles[3].contains(pos) && ellipseHandles[3]->isVisible()){
        return ellipse_rect_left;
    }else{
        return ellipse_rect_no;
    }
}

void myEllipseItem::setNowRect(QRectF nowRect){
    this->nowRect = nowRect;
}

QRectF myEllipseItem::getNowRect(){
    return nowRect;
}















