#include "rectcommand.h"
#include "commandmanager.h"
#include "screenshotview.h"
#include "myrectitem.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

rectCommand::rectCommand()
{

}

rectCommand::~rectCommand(){

}

void rectCommand::mousePressCommand(QMouseEvent *event){
    commandManager::getInstance()->drawRectStart = event->pos();
    commandManager::getInstance()->rectPen.setColor(screenshotView::getInstance()->getControl()->myColorWidget->settings->getRectColor());
    commandManager::getInstance()->rectPen.setWidth(screenshotView::getInstance()->getControl()->myColorWidget->settings->getRectWidth());
    screenshotView::getInstance()->getCurrentRectItem()->setPen(commandManager::getInstance()->rectPen);
}

void rectCommand::mouseMoveCommand(QMouseEvent *event){
    screenshotView::getInstance()->getCurrentRectItem()->setRect(QRectF(commandManager::getInstance()->drawRectStart,event->pos()).normalized());
}

void rectCommand::mouseReleaseCommand(QMouseEvent *event){
    commandManager::getInstance()->drawRectEnd = event->pos();
    if(event->pos() == commandManager::getInstance()->drawRectStart){
        return;
    }
    myRectItem* newRectItem = new myRectItem(QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).intersected(screenshotView::getInstance()->getCurrentRectItem()->rect()));
    newRectItem->setPen(commandManager::getInstance()->rectPen);
    newRectItem->setNowRect(newRectItem->rect());
    screenshotView::getInstance()->getScene()->addItem(newRectItem);
    screenshotView::getInstance()->getCurrentRectItem()->setRect(QRectF());

    order* addOrder = new order();
    addOrder->addToAddItem(newRectItem);
    undoManager* myUndoManager = undoManager::getInstance();
    redoManager* myRedoManager = redoManager::getInstance();
    myUndoManager->pushOrder(addOrder);
    myRedoManager->clear();
}
