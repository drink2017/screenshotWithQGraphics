#include "pencommand.h"
#include "screenshotview.h"
#include "commandmanager.h"
#include "mypenitem.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

#include <QPainterPath>

penCommand::penCommand()
{

}

penCommand::~penCommand(){

}

void penCommand::mousePressCommand(QMouseEvent *event){
    QPainterPath path = screenshotView::getInstance()->getCurrentPenItem()->path();
    path.moveTo(event->pos());
    screenshotView::getInstance()->getCurrentPenItem()->setPath(path);
    commandManager::getInstance()->penPen.setColor(screenshotView::getInstance()->getControl()->myColorWidget->settings->getPenColor());
    commandManager::getInstance()->penPen.setWidth(screenshotView::getInstance()->getControl()->myColorWidget->settings->getPenWidth());
    screenshotView::getInstance()->getCurrentPenItem()->setPen(commandManager::getInstance()->penPen);
}

void penCommand::mouseMoveCommand(QMouseEvent *event){
    QPainterPath path = screenshotView::getInstance()->getCurrentPenItem()->path();

    QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
    if(event->pos().x() < selectRect.left() && event->pos().y() < selectRect.top()){
        path.lineTo(QPointF(selectRect.left(),selectRect.top()));
    }else if(event->pos().x() < selectRect.left() && event->pos().y() > selectRect.bottom()){
        path.lineTo(QPointF(selectRect.left(),selectRect.bottom()));
    }else if(event->pos().x() > selectRect.right() && event->pos().y() < selectRect.top()){
        path.lineTo(QPointF(selectRect.right(),selectRect.top()));
    }else if(event->pos().x() > selectRect.right() && event->pos().y() > selectRect.bottom()){
        path.lineTo(QPointF(selectRect.right(),selectRect.bottom()));
    }else if(event->pos().x() < selectRect.left()){
        path.lineTo(QPointF(selectRect.left(),event->pos().y()));
    }else if(event->pos().y() < selectRect.top()){
        path.lineTo(QPointF(event->pos().x(),selectRect.top()));
    }else if(event->pos().x() > selectRect.right()){
        path.lineTo(QPointF(selectRect.right(),event->pos().y()));
    }else if(event->pos().y() > selectRect.bottom()){
        path.lineTo(QPointF(event->pos().x(),selectRect.bottom()));
    }else{
        path.lineTo(QPointF(event->pos().x(),event->pos().y()));
    }
    screenshotView::getInstance()->getCurrentPenItem()->setPath(path);
}

void penCommand::mouseReleaseCommand(QMouseEvent *event){
    Q_UNUSED(event);
    QGraphicsPathItem* currentPenItem = screenshotView::getInstance()->getCurrentPenItem();
    myPenItem* newPenItem = new myPenItem(currentPenItem->path());
    newPenItem->setPen(currentPenItem->pen());
    screenshotView::getInstance()->getScene()->addItem(newPenItem);
    currentPenItem->setPath(QPainterPath());

    order* addOrder = new order();
    addOrder->addToAddItem(newPenItem);
    undoManager* myUndoManager = undoManager::getInstance();
    redoManager* myRedoManager = redoManager::getInstance();
    myUndoManager->pushOrder(addOrder);
    myRedoManager->clear();
}










