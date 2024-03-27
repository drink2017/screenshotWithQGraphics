#include "controlwidget.h"
#include "screenshotview.h"
#include "undomanager.h"
#include "redomanager.h"
#include "myrectitem.h"
#include "myellipseitem.h"
#include "mypenitem.h"

#include <commandmanager.h>
#include <QGuiApplication>
#include <QScreen>
#include <QApplication>
#include <QStyle>
#include <QDebug>

controlWidget::controlWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

    QPoint selectStart = screenshotView::getInstance()->getSelectStart();
    QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();

    myColorWidget = new colorWidget();
    myColorWidget->hide();

    //设置位置
    setLocation(QRect(selectStart,selectEnd).topLeft(),QRect(selectStart,selectEnd).bottomRight());

    setButtons();
    pLayout = new QHBoxLayout(this);
    pLayout->setSpacing(6);
    pLayout->setContentsMargins(6,6,6,6);

    addButtonsToLayout();
    connectToSlots();
}

void controlWidget::setLocation(QPoint selectStart,QPoint selectEnd){
    QScreen* screen = QGuiApplication::primaryScreen();
    int screenHeight = screen->size().height();
    if(!myColorWidget->isVisible()){
        if(selectEnd.x() < 422){
            if(selectStart.y() < 44 && (screenHeight - selectEnd.y()) < 44){
                locationRect.setRect(0,selectStart.y(),386,44);
            }else if((screenHeight - selectEnd.y()) < 44){
                locationRect.setRect(0,selectStart.y() - 44,386,44);
            }else{
                locationRect.setRect(0,selectEnd.y(),386,44);
            }
        }else{
            if(selectStart.y() < 44 && (screenHeight - selectEnd.y()) < 44){
                locationRect.setRect(selectEnd.x() - 422,selectStart.y(),386,44);
            }else if((screenHeight - selectEnd.y()) < 44){
                locationRect.setRect(selectEnd.x() - 422,selectStart.y() - 44,386,44);
            }else{
                locationRect.setRect(selectEnd.x() - 422,selectEnd.y(),386,44);
            }
        }
    }else{
        if(selectEnd.x() < 422){
            if(selectStart.y() < 98 && (screenHeight - selectEnd.y()) < 98){
                locationRect.setRect(0,selectStart.y(),386,44);
                myColorWidget->setLocationBelow();
            }else if((screenHeight - selectEnd.y()) < 98){
                locationRect.setRect(0,selectStart.y() - 44,386,44);
                myColorWidget->setLocationAbove();
            }else{
                locationRect.setRect(0,selectEnd.y(),386,44);
                myColorWidget->setLocationBelow();
            }
        }else{
            if(selectStart.y() < 98 && (screenHeight - selectEnd.y()) < 98){
                locationRect.setRect(selectEnd.x() - 422,selectStart.y(),386,44);
                myColorWidget->setLocationBelow();
            }else if((screenHeight - selectEnd.y()) < 98){
                locationRect.setRect(selectEnd.x() - 422,selectStart.y() - 44,386,44);
                myColorWidget->setLocationAbove();
            }else{
                locationRect.setRect(selectEnd.x() - 422,selectEnd.y(),386,44);
                myColorWidget->setLocationBelow();
            }
        }
    }
    setGeometry(locationRect);
}

void controlWidget::setButtons(){
    //添加10个按钮
    pRectButton = new QPushButton(this);
    pRectButton->setFixedSize(QSize(32,32));
    pRectButton->setToolTip("矩形");
    pRectButton->setCheckable(true);
    pRectButton->setChecked(false);
    QIcon rectIcon("E:\\software\\qt\\qtProjects\\screenshot3\\icon\\rectangle.png");
    pRectButton->setIcon(rectIcon);

    pRoundButton = new QPushButton(this);
    pRoundButton->setToolTip("圆");
    pRoundButton->setFixedSize(QSize(32,32));
    pRoundButton->setCheckable(true);
    pRoundButton->setChecked(false);
    QIcon roundIcon("E:\\software\\qt\\qtProjects\\screenshot3\\icon\\dry-clean.png");
    pRoundButton->setIcon(roundIcon);

    pArrowButton = new QPushButton(this);
    pArrowButton->setFixedSize(QSize(32,32));
    pArrowButton->setToolTip("箭头");
    pArrowButton->setCheckable(true);
    pArrowButton->setChecked(false);
    QIcon arrowIcon("E:\\software\\qt\\qtProjects\\screenshot3\\icon\\right-arrow.png");
    pArrowButton->setIcon(arrowIcon);

    pPenButton = new QPushButton(this);
    pPenButton->setFixedSize(QSize(32,32));
    pPenButton->setToolTip("画笔");
    pPenButton->setCheckable(true);
    pPenButton->setChecked(false);
    QIcon penIcon("E:\\software\\qt\\qtProjects\\screenshot3\\icon\\pen.png");
    pPenButton->setIcon(penIcon);

    PTextButton = new QPushButton;
    PTextButton->setFixedSize(QSize(32,32));
    PTextButton->setToolTip("文字");
    QIcon textIcon("E:\\software\\qt\\qtProjects\\screenshot3\\icon\\text.png");
    PTextButton->setIcon(textIcon);

    pSerialButton = new QPushButton;
    pSerialButton->setFixedSize(QSize(32,32));
    pSerialButton->setToolTip("序号笔");
    QIcon serialIcon("E:\\software\\qt\\qtProjects\\screenshot2\\icon\\serial.png");
    pSerialButton->setIcon(serialIcon);

    pUndoButton = new QPushButton;
    pUndoButton->setFixedSize(QSize(32,32));
    pUndoButton->setToolTip("撤销");
    QIcon undoIcon = QApplication::style()->standardIcon(QStyle::SP_ArrowBack);
    pUndoButton->setIcon(undoIcon);

    pRedoButton = new QPushButton;
    pRedoButton->setToolTip("重做");
    pRedoButton->setFixedSize(QSize(32,32));
    QIcon redoIcon = QApplication::style()->standardIcon(QStyle::SP_ArrowForward);
    pRedoButton->setIcon(redoIcon);

    pNOButton = new QPushButton;
    pNOButton->setFixedSize(QSize(32,32));
    pNOButton->setToolTip("退出截图");
    QIcon noIcon = QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton);
    pNOButton->setIcon(noIcon);

    pOKButton = new QPushButton;
    pOKButton->setFixedSize(QSize(32,32));
    QIcon okIcon = QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton);
    pOKButton->setIcon(okIcon);
    pOKButton->setToolTip("复制到剪贴板");
}

void controlWidget::addButtonsToLayout(){
    pLayout->addWidget(pRectButton);
    pLayout->addWidget(pRoundButton);
    pLayout->addWidget(pArrowButton);
    pLayout->addWidget(pPenButton);
    pLayout->addWidget(PTextButton);
    pLayout->addWidget(pSerialButton);
    pLayout->addWidget(pUndoButton);
    pLayout->addWidget(pRedoButton);
    pLayout->addWidget(pNOButton);
    pLayout->addWidget(pOKButton);
}

void controlWidget::connectToSlots(){
    connect(pRectButton,&QPushButton::clicked,this,&controlWidget::rectButtonStatu);
    connect(pRoundButton,&QPushButton::clicked,this,&controlWidget::roundButtonStatu);
    connect(pArrowButton,&QPushButton::clicked,this,&controlWidget::arrowButtonStatu);
    connect(pPenButton,&QPushButton::clicked,this,&controlWidget::penButtonStatu);
    connect(pUndoButton,&QPushButton::clicked,this,&controlWidget::undo);
    connect(pRedoButton,&QPushButton::clicked,this,&controlWidget::redo);
    connect(pNOButton,&QPushButton::clicked,this,&controlWidget::quit);
}

void controlWidget::rectButtonStatu(){
    pRoundButton->setChecked(false);
    pArrowButton->setChecked(false);
    pPenButton->setChecked(false);
    if(pRectButton->isChecked()){
        emit enableDrawRect();
        emit disableDrawRound();
        emit disableDrawArrow();
        emit disableDrawPen();
        myColorWidget->setType(widgetType::rect);
        myColorWidget->show();
        QPoint selectStart = screenshotView::getInstance()->getSelectStart();
        QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();
        setLocation(QRect(selectStart,selectEnd).topLeft(),QRect(selectStart,selectEnd).bottomRight());
    }else{
        myColorWidget->hide();
        QPoint selectStart = screenshotView::getInstance()->getSelectStart();
        QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();
        setLocation(QRect(selectStart,selectEnd).topLeft(),QRect(selectStart,selectEnd).bottomRight());
    }
}

void controlWidget::roundButtonStatu(){
    pRectButton->setChecked(false);
    pArrowButton->setChecked(false);
    pPenButton->setChecked(false);
    if(pRoundButton->isChecked()){
        emit enableDrawRound();
        emit disableDrawRect();
        emit disableDrawArrow();
        emit disableDrawPen();
        myColorWidget->setType(widgetType::ellipse);
        myColorWidget->show();
        QPoint selectStart = screenshotView::getInstance()->getSelectStart();
        QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();
        setLocation(QRect(selectStart,selectEnd).topLeft(),QRect(selectStart,selectEnd).bottomRight());
    }else{
        myColorWidget->hide();
        QPoint selectStart = screenshotView::getInstance()->getSelectStart();
        QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();
        setLocation(QRect(selectStart,selectEnd).topLeft(),QRect(selectStart,selectEnd).bottomRight());
    }
}

void controlWidget::arrowButtonStatu(){
    pRectButton->setChecked(false);
    pRoundButton->setChecked(false);
    pPenButton->setChecked(false);
    if(pArrowButton->isChecked()){
        emit enableDrawArrow();
        emit disableDrawRect();
        emit disableDrawRound();
        emit disableDrawPen();
        myColorWidget->setType(widgetType::arrow);
        myColorWidget->show();
        QPoint selectStart = screenshotView::getInstance()->getSelectStart();
        QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();
        setLocation(QRect(selectStart,selectEnd).topLeft(),QRect(selectStart,selectEnd).bottomRight());
    }else{
        myColorWidget->hide();
        QPoint selectStart = screenshotView::getInstance()->getSelectStart();
        QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();
        setLocation(QRect(selectStart,selectEnd).topLeft(),QRect(selectStart,selectEnd).bottomRight());
    }
}

void controlWidget::penButtonStatu(){
    pRectButton->setChecked(false);
    pRoundButton->setChecked(false);
    pArrowButton->setChecked(false);
    if(pPenButton->isChecked()){
        emit enableDrawPen();
        emit disableDrawRect();
        emit disableDrawRound();
        emit disableDrawArrow();
        myColorWidget->setType(widgetType::pen);
        myColorWidget->show();
        QPoint selectStart = screenshotView::getInstance()->getSelectStart();
        QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();
        setLocation(QRect(selectStart,selectEnd).topLeft(),QRect(selectStart,selectEnd).bottomRight());
    }else{
        myColorWidget->hide();
        QPoint selectStart = screenshotView::getInstance()->getSelectStart();
        QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();
        setLocation(QRect(selectStart,selectEnd).topLeft(),QRect(selectStart,selectEnd).bottomRight());
    }
}

void controlWidget::undo(){
    undoManager* myUndoManager = undoManager::getInstance();
    redoManager* myRedoManager = redoManager::getInstance();
    if(myUndoManager->isEmpty()){
        return;
    }else{
        order* undoOrder = myUndoManager->popOrder();
        QQueue<QGraphicsItem*> addItem = undoOrder->getAddItem();
        QQueue<QGraphicsItem*> deleteItem = undoOrder->getDeleteItem();
        if(!addItem.isEmpty() && deleteItem.isEmpty()){
            QGraphicsItem* item = addItem.back();
            screenshotView::getInstance()->getScene()->removeItem(item);
            undoOrder->clearAddItem();
            undoOrder->addToDeleteItem(item);
            myRedoManager->pushOrder(undoOrder);
        }else if(!addItem.isEmpty() && !deleteItem.isEmpty()){
            auto& itemHasDeleted = *(deleteItem.back());
            if(typeid (itemHasDeleted) == typeid (myRectItem)){
                myRectItem* beforeMoveItem = dynamic_cast<myRectItem*>(deleteItem.back());
                myRectItem* afterMoveItem = dynamic_cast<myRectItem*>(addItem.back());
                QRectF afterMoveRect = afterMoveItem->rect();
                QPointF afterMovePos = afterMoveItem->pos();
                afterMoveItem->setRect(beforeMoveItem->rect());
                afterMoveItem->setNowRect(beforeMoveItem->mapToScene(beforeMoveItem->boundingRect()).boundingRect());
                afterMoveItem->setPos(beforeMoveItem->pos());
                afterMoveItem->updateEllipseHandles();
                beforeMoveItem->setRect(afterMoveRect);
                beforeMoveItem->setPos(afterMovePos);
                undoOrder->clearAddItem();
                undoOrder->clearDeleteItem();
                undoOrder->addToAddItem(afterMoveItem);
                undoOrder->addToDeleteItem(beforeMoveItem);
                myRedoManager->pushOrder(undoOrder);
            }else if(typeid (itemHasDeleted) == typeid (myEllipseItem)){
                myEllipseItem* beforeMoveItem = dynamic_cast<myEllipseItem*>(deleteItem.back());
                myEllipseItem* afterMoveItem = dynamic_cast<myEllipseItem*>(addItem.back());
                QRectF afterMoveRect = afterMoveItem->rect();
                QPointF afterMovePos = afterMoveItem->pos();
                afterMoveItem->setRect(beforeMoveItem->rect());
                afterMoveItem->setNowRect(beforeMoveItem->mapToScene(beforeMoveItem->boundingRect()).boundingRect());
                afterMoveItem->setPos(beforeMoveItem->pos());
                afterMoveItem->updateEllipseHandles();
                beforeMoveItem->setRect(afterMoveRect);
                beforeMoveItem->setPos(afterMovePos);
                undoOrder->clearAddItem();
                undoOrder->clearDeleteItem();
                undoOrder->addToAddItem(afterMoveItem);
                undoOrder->addToDeleteItem(beforeMoveItem);
                myRedoManager->pushOrder(undoOrder);
            }else if(typeid (itemHasDeleted) == typeid (myArrowItem)){
                myArrowItem* beforeMoveItem = dynamic_cast<myArrowItem*>(deleteItem.back());
                myArrowItem* afterMoveItem = dynamic_cast<myArrowItem*>(addItem.back());
                QPointF afterMoveStart = afterMoveItem->getStart();
                QPointF afterMoveEnd = afterMoveItem->getEnd();
                afterMoveItem->setStart((afterMoveItem->mapFromScene((beforeMoveItem->mapToScene(beforeMoveItem->getStart())))).toPoint());
                afterMoveItem->setEnd((afterMoveItem->mapFromScene(beforeMoveItem->mapToScene(beforeMoveItem->getEnd()))).toPoint());
                afterMoveItem->updateEllipseHandles();
                beforeMoveItem->setStart((beforeMoveItem->mapFromScene(afterMoveItem->mapToScene(afterMoveStart))).toPoint());
                beforeMoveItem->setEnd((beforeMoveItem->mapFromScene(afterMoveItem->mapToScene(afterMoveEnd))).toPoint());
                undoOrder->clearAddItem();
                undoOrder->clearDeleteItem();
                undoOrder->addToAddItem(afterMoveItem);
                undoOrder->addToDeleteItem(beforeMoveItem);
                myRedoManager->pushOrder(undoOrder);
            }else if(typeid (itemHasDeleted) == typeid (myPenItem)){
                myPenItem* beforeMoveItem = dynamic_cast<myPenItem*>(deleteItem.back());
                myPenItem* afterMoveItem = dynamic_cast<myPenItem*>(addItem.back());
                QPainterPath afterMovePath = afterMoveItem->path();
                afterMoveItem->setPath(afterMoveItem->mapFromScene(beforeMoveItem->mapToScene(beforeMoveItem->path())));
                beforeMoveItem->setPath(beforeMoveItem->mapFromScene(afterMoveItem->mapToScene(afterMovePath)));
                undoOrder->clearAddItem();
                undoOrder->clearDeleteItem();
                undoOrder->addToAddItem(afterMoveItem);
                undoOrder->addToDeleteItem(beforeMoveItem);
                myRedoManager->pushOrder(undoOrder);
            }
        }
    }
}

void controlWidget::redo(){
    undoManager* myUndoManager = undoManager::getInstance();
    redoManager* myRedoManager = redoManager::getInstance();
    if(myRedoManager->isEmpty()){
        return;
    }else{
        order* redoOrder = myRedoManager->popOrder();
        QQueue<QGraphicsItem*> addItem = redoOrder->getAddItem();
        QQueue<QGraphicsItem*> deleteItem = redoOrder->getDeleteItem();
        if(addItem.isEmpty() && !deleteItem.isEmpty()){
            QGraphicsItem* item = deleteItem.back();
            screenshotView::getInstance()->getScene()->addItem(item);
            redoOrder->clearDeleteItem();
            redoOrder->addToAddItem(item);
            myUndoManager->pushOrder(redoOrder);
        }else if(!addItem.isEmpty() && !deleteItem.isEmpty()){
            auto& itemHasDeleted = *(deleteItem.back());
            if(typeid (itemHasDeleted) == typeid (myRectItem)){
                myRectItem* beforeMoveItem = dynamic_cast<myRectItem*>(deleteItem.back());
                myRectItem* afterMoveItem = dynamic_cast<myRectItem*>(addItem.back());
                QRectF afterMoveRect = afterMoveItem->rect();
                QPointF afterMovePos = afterMoveItem->pos();
                afterMoveItem->setRect(beforeMoveItem->rect());
                afterMoveItem->setNowRect(beforeMoveItem->mapToScene(beforeMoveItem->boundingRect()).boundingRect());
                afterMoveItem->setPos(beforeMoveItem->pos());
                afterMoveItem->updateEllipseHandles();
                beforeMoveItem->setRect(afterMoveRect);
                beforeMoveItem->setPos(afterMovePos);
                redoOrder->clearAddItem();
                redoOrder->clearDeleteItem();
                redoOrder->addToAddItem(afterMoveItem);
                redoOrder->addToDeleteItem(beforeMoveItem);
                myUndoManager->pushOrder(redoOrder);
            }else if(typeid (itemHasDeleted) == typeid (myEllipseItem)){
                myEllipseItem* beforeMoveItem = dynamic_cast<myEllipseItem*>(deleteItem.back());
                myEllipseItem* afterMoveItem = dynamic_cast<myEllipseItem*>(addItem.back());
                QRectF afterMoveRect = afterMoveItem->rect();
                QPointF afterMovePos = afterMoveItem->pos();
                afterMoveItem->setRect(beforeMoveItem->rect());
                afterMoveItem->setNowRect(beforeMoveItem->mapToScene(beforeMoveItem->boundingRect()).boundingRect());
                afterMoveItem->setPos(beforeMoveItem->pos());
                afterMoveItem->updateEllipseHandles();
                beforeMoveItem->setRect(afterMoveRect);
                beforeMoveItem->setPos(afterMovePos);
                redoOrder->clearAddItem();
                redoOrder->clearDeleteItem();
                redoOrder->addToAddItem(afterMoveItem);
                redoOrder->addToDeleteItem(beforeMoveItem);
                myUndoManager->pushOrder(redoOrder);
            }else if(typeid (itemHasDeleted) == typeid (myArrowItem)){
                myArrowItem* beforeMoveItem = dynamic_cast<myArrowItem*>(deleteItem.back());
                myArrowItem* afterMoveItem = dynamic_cast<myArrowItem*>(addItem.back());
                QPointF afterMoveStart = afterMoveItem->getStart();
                QPointF afterMoveEnd = afterMoveItem->getEnd();
                afterMoveItem->setStart((afterMoveItem->mapFromScene((beforeMoveItem->mapToScene(beforeMoveItem->getStart())))).toPoint());
                afterMoveItem->setEnd((afterMoveItem->mapFromScene(beforeMoveItem->mapToScene(beforeMoveItem->getEnd()))).toPoint());
                afterMoveItem->updateEllipseHandles();
                beforeMoveItem->setStart((beforeMoveItem->mapFromScene(afterMoveItem->mapToScene(afterMoveStart))).toPoint());
                beforeMoveItem->setEnd((beforeMoveItem->mapFromScene(afterMoveItem->mapToScene(afterMoveEnd))).toPoint());
                redoOrder->clearAddItem();
                redoOrder->clearDeleteItem();
                redoOrder->addToAddItem(afterMoveItem);
                redoOrder->addToDeleteItem(beforeMoveItem);
                myUndoManager->pushOrder(redoOrder);
            }else if(typeid (itemHasDeleted) == typeid (myPenItem)){
                myPenItem* beforeMoveItem = dynamic_cast<myPenItem*>(deleteItem.back());
                myPenItem* afterMoveItem = dynamic_cast<myPenItem*>(addItem.back());
                QPainterPath afterMovePath = afterMoveItem->path();
                afterMoveItem->setPath(afterMoveItem->mapFromScene(beforeMoveItem->mapToScene(beforeMoveItem->path())));
                beforeMoveItem->setPath(beforeMoveItem->mapFromScene(afterMoveItem->mapToScene(afterMovePath)));
                redoOrder->clearAddItem();
                redoOrder->clearDeleteItem();
                redoOrder->addToAddItem(afterMoveItem);
                redoOrder->addToDeleteItem(beforeMoveItem);
                myUndoManager->pushOrder(redoOrder);
            }
        }
    }
}

QRect controlWidget::getLocationRect(){
    return locationRect;
}

void controlWidget::updateButtonStatu(){
    if(commandManager::getInstance()->isDrawingRect()){
        pRectButton->setChecked(true);
    }else{
        pRectButton->setChecked(false);
    }
    if(commandManager::getInstance()->isDrawingEllipse()){
        pRoundButton->setChecked(true);
    }else{
        pRoundButton->setChecked(false);
    }
    if(commandManager::getInstance()->isDrawingArrow()){
        pArrowButton->setChecked(true);
    }else{
        pArrowButton->setChecked(false);
    }
    if(commandManager::getInstance()->isDrawingPen()){
        pPenButton->setChecked(true);
    }else{
        pPenButton->setChecked(false);
    }
}

























