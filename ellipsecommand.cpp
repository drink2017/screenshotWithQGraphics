#include "ellipsecommand.h"
#include "commandmanager.h"
#include "screenshotview.h"
#include "myellipseitem.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

ellipseCommand::ellipseCommand()
{

}

ellipseCommand::~ellipseCommand(){

}

void ellipseCommand::mousePressCommand(QMouseEvent *event){
    commandManager::getInstance()->drawEllipseStart = event->pos();
    commandManager::getInstance()->ellipsePen.setColor(screenshotView::getInstance()->getControl()->myColorWidget->settings->getEllipseColor());
    commandManager::getInstance()->ellipsePen.setWidth(screenshotView::getInstance()->getControl()->myColorWidget->settings->getEllipseWidth());
    screenshotView::getInstance()->getCurrentEllipseItem()->setPen(commandManager::getInstance()->ellipsePen);
}

void ellipseCommand::mouseMoveCommand(QMouseEvent *event){
    screenshotView::getInstance()->getCurrentEllipseItem()->setRect(QRectF(commandManager::getInstance()->drawEllipseStart,event->pos()).normalized());
}

void ellipseCommand::mouseReleaseCommand(QMouseEvent *event){
    commandManager::getInstance()->drawEllipseEnd = event->pos();
    if(event->pos() == commandManager::getInstance()->drawEllipseStart){
        return;
    }
    myEllipseItem* newEllipseItem = new myEllipseItem(QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).intersected(screenshotView::getInstance()->getCurrentEllipseItem()->rect()));
    newEllipseItem->setPen(commandManager::getInstance()->ellipsePen);
    newEllipseItem->setNowRect(newEllipseItem->rect());
    screenshotView::getInstance()->getScene()->addItem(newEllipseItem);
    screenshotView::getInstance()->getCurrentEllipseItem()->setRect(QRectF());

    order* addOrder = new order();
    addOrder->addToAddItem(newEllipseItem);
    undoManager* myUndoManager = undoManager::getInstance();
    redoManager* myRedoManager = redoManager::getInstance();
    myUndoManager->pushOrder(addOrder);
    myRedoManager->clear();
}
