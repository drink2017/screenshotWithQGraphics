#include "textcommand.h"
#include "mytextitem.h"
#include "screenshotview.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

textCommand::textCommand()
{

}

textCommand::~textCommand(){

}

void textCommand::mousePressCommand(QMouseEvent *event){
    Q_UNUSED(event);
}

void textCommand::mouseMoveCommand(QMouseEvent *event){
    Q_UNUSED(event);
}

void textCommand::mouseReleaseCommand(QMouseEvent *event){
    myTextItem* currentTextItem = new myTextItem();
    currentTextItem->setPos(event->pos());
    currentTextItem->setDefaultTextColor(screenshotView::getInstance()->getControl()->myTextWidget->settings->getTextColor());
    QFont font;
    font.setPointSize(screenshotView::getInstance()->getControl()->myTextWidget->settings->getTextSize());
    currentTextItem->setFont(font);
    screenshotView::getInstance()->getScene()->addItem(currentTextItem);
    currentTextItem->setFocus();

    order* addOrder = new order();
    addOrder->addToAddItem(currentTextItem);
    undoManager* myUndoManager = undoManager::getInstance();
    redoManager* myRedoManager = redoManager::getInstance();
    myUndoManager->pushOrder(addOrder);
    myRedoManager->clear();
}

