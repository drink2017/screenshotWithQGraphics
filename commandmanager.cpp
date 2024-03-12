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

bool commandManager::isEditingItem(){
    return  editingItem;
}

void commandManager::setEditingItem(bool editingItem){
    this->editingItem = editingItem;
}

void commandManager::connectToControlWidget(){
    connect(screenshotView::getInstance()->getControl(),&controlWidget::enableDrawRect,this,&commandManager::enableDrawRect);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::disableDrawRect,this,&commandManager::disableDrawRect);
    connect(screenshotView::getInstance()->getControl(),&controlWidget::quit,this,&commandManager::quit);
}

void commandManager::enableDrawRect(){
    drawingRect = true;
}

void commandManager::disableDrawRect(){
    drawingRect = false;
}

void commandManager::quit(){
    QApplication::quit();
}
