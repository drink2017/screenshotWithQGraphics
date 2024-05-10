#include "numbercommand.h"
#include "mynumberitem.h"
#include "commandmanager.h"
#include "screenshotview.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

numberCommand::numberCommand()
{

}

numberCommand::~numberCommand(){

}

void numberCommand::mousePressCommand(QMouseEvent *event){
    Q_UNUSED(event);
}

void numberCommand::mouseMoveCommand(QMouseEvent *event){
    Q_UNUSED(event);
}

void numberCommand::mouseReleaseCommand(QMouseEvent *event){
    myNumberItem* currentNumberItem = new myNumberItem(commandManager::getInstance()->number);
    commandManager::getInstance()->number++;
    currentNumberItem->setPos(event->pos());
    currentNumberItem->text->setDefaultTextColor(screenshotView::getInstance()->getControl()->myTextWidget->settings->getNumberColor());
    screenshotView::getInstance()->getScene()->addItem(currentNumberItem);

    order* addOrder = new order();
    addOrder->addToAddItem(currentNumberItem);
    undoManager* myUndoManager = undoManager::getInstance();
    redoManager* myRedoManager = redoManager::getInstance();
    myUndoManager->pushOrder(addOrder);
    myRedoManager->clear();
}
