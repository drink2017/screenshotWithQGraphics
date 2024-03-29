#include "commandmanager.h"
#include "screenshotview.h"

#include <QApplication>

commandManager* commandManager::instance = nullptr;

commandManager* commandManager::getInstance(){
    if(instance == nullptr){
        instance = new commandManager();
    }
    return instance;
}

commandManager::commandManager(QObject* parent) : QObject(parent)
{

}

bool commandManager::isSelectingArea(){
    return selectingArea;
}

void commandManager::setSelectingArea(bool selectingArea){
    this->selectingArea = selectingArea;
}

bool commandManager::isDrawingRect(){
    return drawingRect;
}

void commandManager::setDrawingRect(bool drawingRect){
    this->drawingRect = drawingRect;
}

bool commandManager::isDrawingEllipse(){
    return drawingEllipse;
}

void commandManager::setDrawingEllipse(bool drawingEllipse){
    this->drawingEllipse = drawingEllipse;
}

bool commandManager::isDrawingArrow(){
    return  drawingArrow;
}

void commandManager::setDrawingArrow(bool drawingArrow){
    this->drawingArrow = drawingArrow;
}

bool commandManager::isDrawingPen(){
    return drawingPen;
}

void commandManager::setDrawingPen(bool drawingPen){
    this->drawingPen = drawingPen;
}

bool commandManager::isDrawingText(){
    return drawingText;
}

void commandManager::setDrawingText(bool drawingText){
    this->drawingText = drawingText;
}

bool commandManager::isEditingItem(){
    return editingItem;
}

void commandManager::setEditingItem(bool editingItem){
    this->editingItem = editingItem;
}

void commandManager::connectToControlWidget(){
    connect(screenshotView::getInstance()->getControl(),&controlWidget::enableDrawRect,this,&commandManager::enableDrawRect);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::disableDrawRect,this,&commandManager::disableDrawRect);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::enableDrawRound,this,&commandManager::enableDrawEllipse);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::disableDrawRound,this,&commandManager::disableDrawEllipse);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::enableDrawArrow,this,&commandManager::enableDrawArrow);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::disableDrawArrow,this,&commandManager::disableDrawArrow);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::enableDrawPen,this,&commandManager::enableDrawPen);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::disableDrawPen,this,&commandManager::disableDrawPen);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::enableDrawText,this,&commandManager::enableDrawText);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::disableDrawText,this,&commandManager::disableDrawText);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::quit,this,&commandManager::quit);
}

void commandManager::enableDrawRect(){
    drawingRect = true;
}

void commandManager::disableDrawRect(){
    drawingRect = false;
}

void commandManager::enableDrawEllipse(){
    drawingEllipse = true;
}

void commandManager::disableDrawEllipse(){
    drawingEllipse = false;
}

void commandManager::enableDrawArrow(){
    drawingArrow = true;
}

void commandManager::disableDrawArrow(){
    drawingArrow = false;
}

void commandManager::enableDrawPen(){
    drawingPen = true;
}

void commandManager::disableDrawPen(){
    drawingPen = false;
}

void commandManager::enableDrawText(){
    drawingText = true;
}

void commandManager::disableDrawText(){
    drawingText = false;
}

void commandManager::quit(){
    QApplication::quit();
}
