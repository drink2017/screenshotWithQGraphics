#include "mytextitem.h"
#include "commandmanager.h"
#include "screenshotview.h"

#include <QCursor>
#include <QDebug>
#include <QTextDocument>

myTextItem::myTextItem(QGraphicsItem* parent) : QGraphicsTextItem(parent)
{
    setFlags(ItemIsMovable | ItemIsSelectable);
    this->setTextInteractionFlags(Qt::TextEditorInteraction);

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
    Q_UNUSED(event);
    if(this->toPlainText().isEmpty()){
        scene()->removeItem(this);
        delete this;
    }
}

























