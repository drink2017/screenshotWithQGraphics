#include "screenshotview.h"
#include "commandmanager.h"
#include "mytextitem.h"
#include "mynumberitem.h"
#include "selectcommand.h"
#include "rectcommand.h"
#include "ellipsecommand.h"
#include "arrowcommand.h"
#include "pencommand.h"
#include "textcommand.h"
#include "numbercommand.h"

#include <QGuiApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QDebug>

screenshotView* screenshotView::instance = nullptr;

screenshotView::screenshotView()
{
    //截图工具全屏显示
    resize(QGuiApplication::primaryScreen()->geometry().width(),QGuiApplication::primaryScreen()->geometry().height());
    setWindowFlags(Qt::FramelessWindowHint);
    scene = new QGraphicsScene(this);
    QPixmap originalPixmap = QGuiApplication::primaryScreen()->grabWindow(0);
    scene->addPixmap(originalPixmap);
    setScene(scene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene->addItem(currentRectItem);
    scene->addItem(currentEllipseItem);
    scene->addItem(currentArrowItem);
    currentArrowItem->setFlag(QGraphicsItem::ItemIsMovable,false);
    scene->addItem(currentPenItem);
}

screenshotView* screenshotView::getInstance(){
    if(instance == nullptr){
        instance = new screenshotView();
    }
    return instance;
}


void screenshotView::mousePressEvent(QMouseEvent *event)
{
    commandManager* state = commandManager::getInstance();
    if(state->isSelectingArea() && event->button() == Qt::LeftButton){
        pCommand = new selectCommand();
        pCommand->mousePressCommand(event);
    }
    if(state->isDrawingRect() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        pCommand = new rectCommand();
        pCommand->mousePressCommand(event);
    }
    if(state->isDrawingEllipse() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        pCommand = new ellipseCommand();
        pCommand->mousePressCommand(event);
    }
    if(state->isDrawingArrow() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        pCommand = new arrowCommand();
        pCommand->mousePressCommand(event);
    }
    if(state->isDrawingPen() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        pCommand = new penCommand();
        pCommand->mousePressCommand(event);
    }
    QGraphicsItem* focusedItem = scene->focusItem();
    focusOnText = qgraphicsitem_cast<myTextItem*>(focusedItem);
    focusOnNumber = qgraphicsitem_cast<myNumberItem*>(focusedItem) || qgraphicsitem_cast<myNumberTextItem*>(focusedItem);
    QGraphicsView::mousePressEvent(event);
}

void screenshotView::mouseMoveEvent(QMouseEvent *event)
{
    commandManager* state = commandManager::getInstance();
    if(state->isSelectingArea() && event->buttons() == Qt::LeftButton){
        pCommand->mouseMoveCommand(event);
    }
    if(state->isDrawingRect() && event->buttons() == Qt::LeftButton && !state->isEditingItem()){
        pCommand->mouseMoveCommand(event);
    }
    if(state->isDrawingEllipse() && event->buttons() == Qt::LeftButton && !state->isEditingItem()){
        pCommand->mouseMoveCommand(event);
    }
    if(state->isDrawingArrow() && event->buttons() == Qt::LeftButton && !state->isEditingItem()){
        pCommand->mouseMoveCommand(event);
    }
    if(state->isDrawingPen() && event->buttons() == Qt::LeftButton && !state->isEditingItem()){
        pCommand->mouseMoveCommand(event);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void screenshotView::mouseReleaseEvent(QMouseEvent *event)
{
    commandManager* state = commandManager::getInstance();
    if(state->isSelectingArea() && event->button() == Qt::LeftButton){
        pCommand->mouseReleaseCommand(event);
        delete pCommand;
    }
    if(state->isDrawingRect() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        pCommand->mouseReleaseCommand(event);
        delete pCommand;
    }
    if(state->isDrawingEllipse() && event->button() == Qt::LeftButton && !state->isEditingItem()){   
        pCommand->mouseReleaseCommand(event);
        delete pCommand;
    }
    if(state->isDrawingArrow() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        pCommand->mouseReleaseCommand(event);
        delete pCommand;
    }
    if(state->isDrawingPen() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        pCommand->mouseReleaseCommand(event);
        delete pCommand;
    }
    if(state->isDrawingText() && event->button() == Qt::LeftButton && !state->isEditingItem() && !focusOnText){
        pCommand = new textCommand();
        pCommand->mouseReleaseCommand(event);
        delete pCommand;
    }
    if(state->isDrawingNumber() && event->button() == Qt::LeftButton && !state->isEditingItem() && !focusOnNumber){
        pCommand = new numberCommand();
        pCommand->mouseReleaseCommand(event);
        delete pCommand;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

QPainterPath screenshotView::getShadowPath(QPoint selectStart,QPoint selectEnd){
    QPainterPath globalPath;
    globalPath.lineTo(this->width(),0);
    globalPath.lineTo(this->width(),this->height());
    globalPath.lineTo(0,this->height());
    globalPath.closeSubpath();
    QPainterPath shadowPath;
    shadowPath.moveTo(selectStart);
    shadowPath.lineTo(selectEnd.x(),selectStart.y());
    shadowPath.lineTo(selectEnd);
    shadowPath.lineTo(selectStart.x(),selectEnd.y());
    shadowPath.closeSubpath();
    return globalPath.subtracted(shadowPath);
}

QPoint screenshotView::getSelectStart(){
    return selectStart;
}

QPoint screenshotView::getSelectEnd(){
    return selectEnd;
}

controlWidget* screenshotView::getControl(){
    return control;
}

void screenshotView::setSelectStart(QPointF selectStart){
    this->selectStart = QPoint(selectStart.x(),selectStart.y());
}

void screenshotView::setSelectEnd(QPointF selectEnd){
    this->selectEnd = QPoint(selectEnd.x(),selectEnd.y());
}

infoLabel* screenshotView::getInfo(){
    return info;
}

QGraphicsScene* screenshotView::getScene(){
    return scene;
}

void screenshotView::setCurrentSelectItem(selectItem *selectArea){
    currentSelectItem = selectArea;
}

selectItem* screenshotView::getCurrentSelectItem(){
    return currentSelectItem;
}

void screenshotView::setInfo(infoLabel *info){
    this->info = info;
}

void screenshotView::setControl(controlWidget *control){
    this->control = control;
}

QGraphicsRectItem* screenshotView::getCurrentRectItem(){
    return currentRectItem;
}

QGraphicsEllipseItem* screenshotView::getCurrentEllipseItem(){
    return currentEllipseItem;
}

myArrowItem* screenshotView::getCurrentArrowItem(){
    return currentArrowItem;
}

QGraphicsPathItem* screenshotView::getCurrentPenItem(){
    return currentPenItem;
}













