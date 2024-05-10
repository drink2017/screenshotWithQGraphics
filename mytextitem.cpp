#include "mytextitem.h"
#include "commandmanager.h"
#include "screenshotview.h"
#include "undomanager.h"
#include "redomanager.h"

#include <QDebug>
#include <QTextDocument>

myTextItem::myTextItem(QGraphicsItem* parent) : QGraphicsTextItem(parent)
{
    setFlags(ItemIsMovable | ItemIsSelectable);
    setTextInteractionFlags(Qt::TextEditorInteraction);

    setAcceptHoverEvents(true);

    connect(document(),&QTextDocument::contentsChanged,this,&myTextItem::adjustTextWidth);
}


void myTextItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if(!this->hasFocus()){
        setCursor(Qt::SizeAllCursor);
    }
}

void myTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    commandManager::getInstance()->setEditingItem(true);
    QGraphicsItem::mousePressEvent(event);

    myTextItem* moveBeforeItem = new myTextItem;
    moveBeforeItem->setPos(this->pos());
    moveBeforeItem->setPlainText(moveBeforeItem->toPlainText());
    order* moveOrder = new order();
    moveOrder->addToDeleteItem(moveBeforeItem);
    undoManager::getInstance()->pushOrder(moveOrder);
}

void myTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
    QRectF textRect = this->mapToScene(this->boundingRect()).boundingRect();
    if(textRect.x() < selectRect.x() && textRect.y() < selectRect.y()){
        setPos(selectRect.x(),selectRect.y());
    }else if(textRect.x() < selectRect.x() && textRect.bottom() > selectRect.bottom()){
        setPos(selectRect.x(),selectRect.bottom()-textRect.height());
    }else if(textRect.right() > selectRect.right() && textRect.top() < selectRect.top()){
        setPos(selectRect.right()-textRect.width(),selectRect.top());
    }else if(textRect.right() > selectRect.right() && textRect.bottom() > selectRect.bottom()){
        setPos(selectRect.right()-textRect.width(),selectRect.bottom()-textRect.height());
    }else if(textRect.left() < selectRect.left()){
        setX(selectRect.x());
    }else if(textRect.top() < selectRect.top()){
        setY(selectRect.top());
    }else if(textRect.right() > selectRect.right()){
        setX(selectRect.right()-textRect.width());
    }else if(textRect.bottom() > selectRect.bottom()){
        setY(selectRect.bottom()-textRect.height());
    }
}

void myTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    commandManager::getInstance()->setEditingItem(false);

    order* moveOrder = undoManager::getInstance()->popOrder();
    myTextItem* moveBeforeItem = dynamic_cast<myTextItem*>(moveOrder->getDeleteItem().back());

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

void myTextItem::adjustTextWidth(){
    QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
    QRectF textRect = this->mapToScene(this->boundingRect()).boundingRect();
    if(textRect.right() >= selectRect.right()){
        setTextWidth(selectRect.right() - this->pos().x());
    }else{
        setTextWidth(-1);
    }
}

void myTextItem::focusOutEvent(QFocusEvent *event)
{
    QGraphicsTextItem::focusOutEvent(event);
    if(this->toPlainText().isEmpty()){
        scene()->removeItem(this);
        if(!document()->isModified()){
            undoManager* myUndoManager = undoManager::getInstance();
            order* undoOrder = myUndoManager->popOrder();
            delete undoOrder;
        }else{
            redoManager* myRedoManager = redoManager::getInstance();
            myRedoManager->clear();
        }
    }
}

void myTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF textRect = boundingRect();
    //QRectF backgroundRect = textRect.adjusted(-10, -5, 10, 5); // 在文字周围增加一些内边距

    // 绘制背景矩形
    //painter->setBrush(QBrush(QColor(0, 0, 0, 180))); // 设置半透明黑色背景
    painter->setBrush(QBrush(Qt::white));
    painter->setPen(Qt::NoPen);
    //painter->drawRect(backgroundRect);
    painter->drawRect(textRect);

    QGraphicsTextItem::paint(painter,option,widget);

    screenshotView::getInstance()->getScene()->update();
}
