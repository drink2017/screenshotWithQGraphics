#include "arrowcommand.h"
#include "screenshotview.h"
#include "commandmanager.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

arrowCommand::arrowCommand()
{

}

arrowCommand::~arrowCommand(){

}

void arrowCommand::mousePressCommand(QMouseEvent *event){
    if(!QRect(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).contains(event->pos())){
        QPoint start(event->pos());
        QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
        if(event->pos().x() < selectRect.left()){
            start.setX(selectRect.left());
        }else if(event->pos().x() > selectRect.right()){
            start.setX(selectRect.right());
        }
        if(event->pos().y() < selectRect.top()){
            start.setY(selectRect.top());
        }else if(event->pos().y() > selectRect.bottom()){
            start.setY(selectRect.bottom());
        }
        commandManager::getInstance()->drawArrowStart = start;
    }else{
        commandManager::getInstance()->drawArrowStart = event->pos();
    }
    commandManager::getInstance()->drawArrowEnd = event->pos();
    commandManager::getInstance()->arrowPen.setColor(screenshotView::getInstance()->getControl()->myColorWidget->settings->getArrowColor());
    commandManager::getInstance()->arrowPen.setWidth(screenshotView::getInstance()->getControl()->myColorWidget->settings->getArrowWidth());
    screenshotView::getInstance()->getCurrentArrowItem()->setPen(commandManager::getInstance()->arrowPen);
}

void arrowCommand::mouseMoveCommand(QMouseEvent *event){
    screenshotView::getInstance()->getCurrentArrowItem()->setStart(commandManager::getInstance()->drawArrowStart);

    QPoint end = event->pos();
    QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
    if(event->pos().x() < selectRect.left()){
        end.setX(selectRect.left());
    }else if(event->pos().x() > selectRect.right()){
        end.setX(selectRect.right());
    }
    if(event->pos().y() < selectRect.top()){
        end.setY(selectRect.top());
    }else if(event->pos().y() > selectRect.bottom()){
        end.setY(selectRect.bottom());
    }

    screenshotView::getInstance()->getCurrentArrowItem()->setEnd(end);
}

void arrowCommand::mouseReleaseCommand(QMouseEvent *event){
    QPoint end = event->pos();
    QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
    if(event->pos().x() < selectRect.left()){
        end.setX(selectRect.left());
    }else if(event->pos().x() > selectRect.right()){
        end.setX(selectRect.right());
    }
    if(event->pos().y() < selectRect.top()){
        end.setY(selectRect.top());
    }else if(event->pos().y() > selectRect.bottom()){
        end.setY(selectRect.bottom());
    }
    if(end == commandManager::getInstance()->drawArrowStart){
        return;
    }
    commandManager::getInstance()->drawArrowEnd = end;
    myArrowItem* newArrowItem = new myArrowItem(commandManager::getInstance()->drawArrowStart,commandManager::getInstance()->drawArrowEnd);
    newArrowItem->setPen(commandManager::getInstance()->arrowPen);
    screenshotView::getInstance()->getScene()->addItem(newArrowItem);
    screenshotView::getInstance()->getCurrentArrowItem()->setStart(QPoint());
    screenshotView::getInstance()->getCurrentArrowItem()->setEnd(QPoint());

    order* addOrder = new order();
    addOrder->addToAddItem(newArrowItem);
    undoManager* myUndoManager = undoManager::getInstance();
    redoManager* myRedoManager = redoManager::getInstance();
    myUndoManager->pushOrder(addOrder);
    myRedoManager->clear();
}














