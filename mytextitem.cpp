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

























