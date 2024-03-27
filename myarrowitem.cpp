#include "myarrowitem.h"
#include "screenshotview.h"
#include "commandmanager.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

#include <QPainter>
#include <cmath>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

myArrowItem::myArrowItem()
{

}

myArrowItem::myArrowItem(QPointF start_pos, QPointF end_pos,QGraphicsItem* parent):QGraphicsItem(parent),start_pos(start_pos),end_pos(end_pos){
    setAcceptHoverEvents(true);
    createEllipseHandles();
    hideEllipseHandles();

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
}


QRectF myArrowItem::boundingRect() const
{
    return shape().boundingRect();
}

void myArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    arrowPen.setCapStyle(Qt::RoundCap);
    painter->setPen(arrowPen);

    painter->drawLine(start_pos,end_pos);

    int size = painter->pen().width() * 10;

    double angle = std::atan2(-1.0 * (end_pos.y() - start_pos.y()), end_pos.x() - start_pos.x());
    QPoint arrowP1 = end_pos.toPoint() + QPoint(-1 * size * std::cos(angle - M_PI / 6), size * std::sin(angle - M_PI / 6));
    QPoint arrowP2 = end_pos.toPoint() + QPoint(-1 * size * std::cos(angle + M_PI / 6), size * std::sin(angle + M_PI / 6));

    // 画箭头尖端
    QPainterPath path;
    path.moveTo(end_pos);
    path.lineTo(arrowP1);
    path.lineTo(arrowP2);
    path.closeSubpath();

    // 填充箭头尖端
    painter->fillPath(path, painter->pen().color());

    if(option->state & QStyle::State_Selected){
        showEllipseHandles();
    }else{
        if(!ellipseHandles.isEmpty()){
            hideEllipseHandles();
        }
    }
}

QPainterPath myArrowItem::shape() const
{
    QPainterPath path;
    path.moveTo(start_pos);
    path.lineTo(end_pos);

    int size = arrowPen.width() * 10;
    double angle = std::atan2(-1.0 * (end_pos.y() - start_pos.y()), end_pos.x() - start_pos.x());
    QPoint arrowP1 = end_pos.toPoint() + QPoint(-1 * size * std::cos(angle - M_PI / 6), size * std::sin(angle - M_PI / 6));
    QPoint arrowP2 = end_pos.toPoint() + QPoint(-1 * size * std::cos(angle + M_PI / 6), size * std::sin(angle + M_PI / 6));
    path.lineTo(arrowP1);
    path.lineTo(arrowP2);
    path.lineTo(end_pos);

    QPainterPathStroker stroker;
    stroker.setWidth(4);

    return stroker.createStroke(path);
}

void myArrowItem::setPen(QPen commandPen){
    arrowPen.setWidth(commandPen.width());
    arrowPen.setColor(commandPen.color());
}

void myArrowItem::setStart(QPoint startPoint){
    this->start_pos = startPoint;
    if(this->scene() != nullptr){
        this->scene()->update();
    }
}

void myArrowItem::setEnd(QPoint endPoint){
    this->end_pos = endPoint;
    if(this->scene() != nullptr){
        this->scene()->update();
    }
}

void myArrowItem::createEllipseHandles(){
    for(int i = 0;i < 2;i++){
        QGraphicsEllipseItem* ellipseHandle = new QGraphicsEllipseItem(0,0,10,10,this);
        ellipseHandle->setBrush(QBrush(Qt::white));
        ellipseHandle->setPen(QPen(Qt::red));
        ellipseHandles.append(ellipseHandle);
    }
    updateEllipseHandles();
}

void myArrowItem::updateEllipseHandles(){
    QList<QRectF> handles = {
        QRectF(start_pos.x()-5,start_pos.y()-5,10,10),
        QRectF(end_pos.x()-5,end_pos.y()-5,10,10)
    };

    for(int i = 0;i < 2;i++){
        ellipseHandles[i]->setRect(handles[i]);
    }
}

void myArrowItem::showEllipseHandles(){
    for(int i = 0;i < 2;i++){
        ellipseHandles[i]->show();
    }
}

void myArrowItem::hideEllipseHandles(){
    for(int i = 0;i < 2;i++){
        ellipseHandles[i]->hide();
    }
}

pointInArrow myArrowItem::mousePointIn(QPointF pos){
    QList<QRectF> handles = {
        QRectF(start_pos.x()-5,start_pos.y()-5,10,10),
        QRectF(end_pos.x()-5,end_pos.y()-5,10,10)
    };

    if(handles[0].contains(pos) && ellipseHandles[0]->isVisible()){
        return arrow_start;
    }else if(handles[1].contains(pos) && ellipseHandles[1]->isVisible()){
        return arrow_end;
    }else{
        return arrow_no;
    }
}

void myArrowItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(mousePointIn(event->pos()) == arrow_start || mousePointIn(event->pos()) == arrow_end){
        setCursor(Qt::SizeVerCursor);
    }else{
        setCursor(Qt::SizeAllCursor);
    }
}

void myArrowItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    commandManager::getInstance()->setEditingItem(true);
    type = mousePointIn(event->pos());

    myArrowItem* moveBeforeItem = new myArrowItem(mapToScene(this->getStart()),mapToScene(this->getEnd()));
    order* moveOrder = new order();
    moveOrder->addToDeleteItem(moveBeforeItem);
    undoManager::getInstance()->pushOrder(moveOrder);
}

void myArrowItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(this->isSelected() && type == arrow_no){
        QGraphicsItem::mouseMoveEvent(event);
    }
    if(type == arrow_start){
        setCursor(Qt::SizeVerCursor);
        QPoint start = event->pos().toPoint();
        QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
        selectRect.translate(this->pos().x() * -1,this->pos().y() * -1);
        if(start.x() < selectRect.left()){
            start.setX(selectRect.left());
        }else if(start.x() > selectRect.right()){
            start.setX(selectRect.right());
        }
        if(start.y() < selectRect.top()){
            start.setY(selectRect.y());
        }else if(start.y() > selectRect.bottom()){
            start.setY(selectRect.bottom());
        }
        setStart(start);
        updateEllipseHandles();
    }else if(type == arrow_end){
        setCursor(Qt::SizeVerCursor);

        QPoint end = event->pos().toPoint();
        QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
        selectRect.translate(this->pos().x() * -1,this->pos().y() * -1);
        if(end.x() < selectRect.left()){
            end.setX(selectRect.left());
        }else if(end.x() > selectRect.right()){
            end.setX(selectRect.right());
        }
        if(end.y() < selectRect.top()){
            end.setY(selectRect.top());
        }else if(end.y() > selectRect.bottom()){
            end.setY(selectRect.bottom());
        }
        setEnd(end);

        updateEllipseHandles();
    }
}

void myArrowItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    if(this->isSelected()){
        QGraphicsItem::mouseReleaseEvent(event);
        QRectF sceneRect = this->mapToScene(this->boundingRect()).boundingRect();
        QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
        if(sceneRect.left() < selectRect.left()){
            this->setStart(QPoint(start_pos.x()+selectRect.left()-sceneRect.left(),start_pos.y()));
            this->setEnd(QPoint(end_pos.x()+selectRect.left()-sceneRect.left(),end_pos.y()));
            this->updateEllipseHandles();
        }
        if(sceneRect.top() < selectRect.top()){
            this->setStart(QPoint(start_pos.x(),start_pos.y()+selectRect.top()-sceneRect.top()));
            this->setEnd(QPoint(end_pos.x(),end_pos.y()+selectRect.top()-sceneRect.top()));
            this->updateEllipseHandles();
        }
        if(sceneRect.right() > selectRect.right()){
            this->setStart(QPoint(start_pos.x()+selectRect.right()-sceneRect.right(),start_pos.y()));
            this->setEnd(QPoint(end_pos.x()+selectRect.right()-sceneRect.right(),end_pos.y()));
            this->updateEllipseHandles();
        }
        if(sceneRect.bottom() > selectRect.bottom()){
            this->setStart(QPoint(start_pos.x(),start_pos.y()+selectRect.bottom()-sceneRect.bottom()));
            this->setEnd(QPoint(end_pos.x(),end_pos.y()+selectRect.bottom()-sceneRect.bottom()));
            this->updateEllipseHandles();
        }
        commandManager::getInstance()->setEditingItem(false);

        order* moveOrder = undoManager::getInstance()->popOrder();
        myArrowItem* moveBeforeItem = dynamic_cast<myArrowItem*>(moveOrder->getDeleteItem().back());

        QRect beforeRect = moveBeforeItem->mapToScene(moveBeforeItem->boundingRect()).boundingRect().toRect();
        QRect afterRect = this->mapToScene(this->boundingRect()).boundingRect().toRect();

        if(beforeRect.x() == afterRect.x() && beforeRect.y() == afterRect.y() && beforeRect.width() == afterRect.width() && beforeRect.height() == afterRect.height()){
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

QPointF myArrowItem::getStart(){
    return start_pos;
}

QPointF myArrowItem::getEnd(){
    return end_pos;
}






















