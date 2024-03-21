#include "screenshotview.h"
#include "commandmanager.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"
#include "myrectitem.h"
#include "myellipseitem.h"

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
        selectStart = event->pos();
        selectEnd = event->pos();
        currentSelectItem = new selectItem();
        currentSelectItem->setPath(getShadowPath(selectStart,selectEnd));
        scene->addItem(currentSelectItem);

        info = new infoLabel();
        info->show();
    }
    if(state->isDrawingRect() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        commandManager::getInstance()->drawRectStart = event->pos();
        commandManager::getInstance()->rectPen.setColor(control->myColorWidget->settings->getRectColor());
        commandManager::getInstance()->rectPen.setWidth(control->myColorWidget->settings->getRectWidth());
        currentRectItem->setPen(commandManager::getInstance()->rectPen);
    }
    if(state->isDrawingEllipse() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        commandManager::getInstance()->drawEllipseStart = event->pos();
        commandManager::getInstance()->ellipsePen.setColor(control->myColorWidget->settings->getEllipseColor());
        commandManager::getInstance()->ellipsePen.setWidth(control->myColorWidget->settings->getEllipseWidth());
        currentEllipseItem->setPen(commandManager::getInstance()->ellipsePen);
    }if(state->isDrawingArrow() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        commandManager::getInstance()->drawArrowStart = event->pos();
        commandManager::getInstance()->drawArrowEnd = event->pos();
        commandManager::getInstance()->arrowPen.setColor(control->myColorWidget->settings->getArrowColor());
        commandManager::getInstance()->arrowPen.setWidth(control->myColorWidget->settings->getArrowWidth());
        currentArrowItem->setPen(commandManager::getInstance()->arrowPen);
        currentArrowItem->setStart(commandManager::getInstance()->drawArrowStart);
        currentArrowItem->setEnd(commandManager::getInstance()->drawArrowEnd);
    }
    QGraphicsView::mousePressEvent(event);
}

void screenshotView::mouseMoveEvent(QMouseEvent *event)
{
    commandManager* state = commandManager::getInstance();
    if(state->isSelectingArea() && event->buttons() == Qt::LeftButton){
        selectEnd = event->pos();
        currentSelectItem->setPath(getShadowPath(selectStart,selectEnd));
        currentSelectItem->updateRectHandles();

        info->updateInfoLabel();
    }
    if(state->isDrawingRect() && event->buttons() == Qt::LeftButton && !state->isEditingItem()){
        currentRectItem->setRect(QRectF(commandManager::getInstance()->drawRectStart,event->pos()).normalized());
    }
    if(state->isDrawingEllipse() && event->buttons() == Qt::LeftButton && !state->isEditingItem()){
        currentEllipseItem->setRect(QRectF(commandManager::getInstance()->drawEllipseStart,event->pos()).normalized());
    }
    if(state->isDrawingArrow() && event->buttons() == Qt::LeftButton && !state->isEditingItem()){
        currentArrowItem->setEnd(event->pos());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void screenshotView::mouseReleaseEvent(QMouseEvent *event)
{
    commandManager* state = commandManager::getInstance();
    if(state->isSelectingArea() && event->button() == Qt::LeftButton){
        currentSelectItem->setHover(true);
        currentSelectItem->setFinishSelect();
        commandManager::getInstance()->setSelectingArea(false);
        control = new controlWidget();
        commandManager::getInstance()->connectToControlWidget();
        control->show();
    }
    if(state->isDrawingRect() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        commandManager::getInstance()->drawRectEnd = event->pos();
        myRectItem* newRectItem = new myRectItem(QRectF(selectStart,selectEnd).intersected(currentRectItem->rect()));
        newRectItem->setPen(commandManager::getInstance()->rectPen);
        newRectItem->setNowRect(newRectItem->rect());
        scene->addItem(newRectItem);
        currentRectItem->setRect(QRectF());

        order* addOrder = new order();
        addOrder->addToAddItem(newRectItem);
        undoManager* myUndoManager = undoManager::getInstance();
        redoManager* myRedoManager = redoManager::getInstance();
        myUndoManager->pushOrder(addOrder);
        myRedoManager->clear();
    }
    if(state->isDrawingEllipse() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        commandManager::getInstance()->drawEllipseEnd = event->pos();
        myEllipseItem* newEllipseItem = new myEllipseItem(QRectF(selectStart,selectEnd).intersected(currentEllipseItem->rect()));
        newEllipseItem->setPen(commandManager::getInstance()->ellipsePen);
        newEllipseItem->setNowRect(newEllipseItem->rect());
        scene->addItem(newEllipseItem);
        currentEllipseItem->setRect(QRectF());

        order* addOrder = new order();
        addOrder->addToAddItem(newEllipseItem);
        undoManager* myUndoManager = undoManager::getInstance();
        redoManager* myRedoManager = redoManager::getInstance();
        myUndoManager->pushOrder(addOrder);
        myRedoManager->clear();
    }
    if(state->isDrawingArrow() && event->button() == Qt::LeftButton && !state->isEditingItem()){
        commandManager::getInstance()->drawArrowEnd = event->pos();
        myArrowItem* newArrowItem = new myArrowItem(commandManager::getInstance()->drawArrowStart,commandManager::getInstance()->drawArrowEnd);
        newArrowItem->setPen(commandManager::getInstance()->arrowPen);
        scene->addItem(newArrowItem);
        currentArrowItem->setStart(QPoint());
        currentArrowItem->setEnd(QPoint());
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















