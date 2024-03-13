#include "screenshotview.h"
#include "commandmanager.h"
#include "order.h"
#include "undomanager.h"
#include "redomanager.h"

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
        if(event->pos().x() < commandManager::getInstance()->drawRectStart.x() && event->pos().y() < commandManager::getInstance()->drawRectStart.y()){
            currentRectItem->setRect(QRect(event->pos(),commandManager::getInstance()->drawRectStart));
        }else if(event->pos().x() < commandManager::getInstance()->drawRectStart.x()){
            currentRectItem->setRect(QRect(QPoint(event->pos().x(),commandManager::getInstance()->drawRectStart.y()),QPoint(commandManager::getInstance()->drawRectStart.x(),event->pos().y())));
        }else if(event->pos().y() < commandManager::getInstance()->drawRectStart.y()){
            currentRectItem->setRect(QRect(QPoint(commandManager::getInstance()->drawRectStart.x(),event->pos().y()),QPoint(event->pos().x(),commandManager::getInstance()->drawRectStart.y())));
        }else{
            currentRectItem->setRect(QRect(commandManager::getInstance()->drawRectStart,event->pos()));
        }
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
        scene->addItem(newRectItem);

        currentRectItem->setRect(QRectF());

        order* addOrder = new order();
        addOrder->addToAddItem(newRectItem);
        undoManager* myUndoManager = undoManager::getInstance();
        redoManager* myRedoManager = redoManager::getInstance();
        myUndoManager->pushOrder(addOrder);
        myRedoManager->clear();
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















