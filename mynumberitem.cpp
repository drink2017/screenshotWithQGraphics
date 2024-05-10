#include "mynumberitem.h"
#include "screenshotview.h"
#include "commandmanager.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

#include <QFont>
#include <QDebug>

myNumberItem::myNumberItem(int number,QGraphicsItem* parent):QGraphicsItemGroup(parent)
{
    setHandlesChildEvents(false);
    setAcceptHoverEvents(true);

    ellipse = new QGraphicsEllipseItem(-11,-11,22,22,this);
    ellipse->setBrush(QBrush(screenshotView::getInstance()->getControl()->myTextWidget->settings->getNumberColor()));

    serial = new QGraphicsSimpleTextItem(QString::number(number),this);
    if(number < 10){
        serial->setPos(-6,-12);
        serial->setFont(QFont("Arial",11));
    }else{
        serial->setPos(-9,-9);
        serial->setFont(QFont("ARial",9));
    }
    if(ellipse->brush().color() == Qt::white || ellipse->brush().color() == Qt::yellow){
        serial->setBrush(QColor(Qt::black));
    }else{
        serial->setBrush(QColor(Qt::white));
    }

    text = new myNumberTextItem();
    text->setPos(20,-15);
    text->setFocus();

    addToGroup(ellipse);
    addToGroup(serial);
    addToGroup(text);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
}

void myNumberItem::setColor(QColor color){
    ellipse->setBrush(QBrush(color));
    if(color == Qt::white || color == Qt::yellow){
        serial->setBrush(QColor(Qt::black));
    }else{
        serial->setBrush(QColor(Qt::white));
    }
}

void myNumberItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    commandManager::getInstance()->setEditingItem(true);
    QGraphicsItem::mousePressEvent(event);

    myNumberItem* moveBeforeItem = new myNumberItem(this->serial->text().toInt());
    moveBeforeItem->setPos(this->pos());
    moveBeforeItem->text->setPlainText(this->text->toPlainText());
    order* moveOrder = new order();
    moveOrder->addToDeleteItem(moveBeforeItem);
    undoManager::getInstance()->pushOrder(moveOrder);
}

void myNumberItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
    QRectF groupRect = this->mapToScene(this->boundingRect()).boundingRect();
    QRectF textRect = this->text->mapToScene(this->text->boundingRect()).boundingRect();
    if(groupRect.left() < selectRect.left() && groupRect.top() < selectRect.top()){
        setPos(selectRect.topLeft()-boundingRect().topLeft());
    }else if(groupRect.left() < selectRect.left() && textRect.bottom() > selectRect.bottom()){
        setPos(QPointF(selectRect.x()-boundingRect().left(),selectRect.y()+selectRect.height()-textRect.height()-boundingRect().top()));
    }else if(textRect.right() > selectRect.right() && groupRect.top() < selectRect.top()){
        setPos(QPointF(selectRect.right()-(textRect.left()-groupRect.left()+textRect.width()+boundingRect().left()),selectRect.top()-boundingRect().top()));
    }else if(textRect.right() > selectRect.right() && textRect.bottom() > selectRect.bottom()){
        setPos(QPointF(selectRect.right()-(textRect.left()-groupRect.left()+textRect.width()+boundingRect().left()),selectRect.bottom()-textRect.height()-boundingRect().top()));
    }else if(groupRect.left() < selectRect.left()){
        setX(selectRect.x()-boundingRect().left());
    }else if(groupRect.top() < selectRect.top()){
        setY(selectRect.top()-boundingRect().top());
    }else if(textRect.right() > selectRect.right()){
        setX(selectRect.right()-(textRect.left()-groupRect.left()+textRect.width()+boundingRect().left()));
    }else if(textRect.bottom() > selectRect.bottom()){
        setY(selectRect.bottom()-textRect.height()-boundingRect().top());
    }
}

void myNumberItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    commandManager::getInstance()->setEditingItem(false);

    order* moveOrder = undoManager::getInstance()->popOrder();
    myNumberItem* moveBeforeItem = dynamic_cast<myNumberItem*>(moveOrder->getDeleteItem().back());

    if(this->pos() == moveBeforeItem->pos()){
        QQueue<QGraphicsItem*> deleteItem = moveOrder->getDeleteItem();
        delete deleteItem.back();
        delete moveOrder;
    }else{
        moveOrder->addToAddItem(this);
        undoManager::getInstance()->pushOrder(moveOrder);
        redoManager::getInstance()->clear();
    }
}

void myNumberItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::SizeAllCursor);
}
