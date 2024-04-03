#include "mynumberitem.h"
#include "screenshotview.h"
#include "commandmanager.h"

#include <QFont>
#include <QDebug>

myNumberItem::myNumberItem(int number,QGraphicsItem* parent):QGraphicsItemGroup(parent)
{
    setHandlesChildEvents(false);

    ellipse = new QGraphicsEllipseItem(-15,-15,30,30,this);
    ellipse->setBrush(QBrush(screenshotView::getInstance()->getControl()->myTextWidget->settings->getNumberColor()));

    serial = new QGraphicsSimpleTextItem(QString::number(number),this);
    if(number < 10){
        serial->setPos(-8,-16);
        serial->setFont(QFont("Arial",15));
    }else{
        serial->setPos(-12,-12);
        serial->setFont(QFont("ARial",12));
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
}

void myNumberItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void myNumberItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    commandManager::getInstance()->setEditingItem(false);
}


















