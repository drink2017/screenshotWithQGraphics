﻿#include "selectitem.h"
#include "screenshotview.h"
#include "commandmanager.h"
#include "myrectitem.h"
#include "myellipseitem.h"
#include "myarrowitem.h"
#include "mypenitem.h"
#include "mytextitem.h"
#include "mynumberitem.h"
#include "mynumbertextitem.h"
#include "undomanager.h"
#include "redomanager.h"

#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

selectItem::selectItem()
{
    setPen(QPen(QColor(135, 206, 250)));
    setBrush(QBrush(QColor(0,0,0,100)));
    createRectHandles();
}

void selectItem::createRectHandles(){
    for(int i = 0;i < 8;i++){
        QGraphicsRectItem* rectHandle = new QGraphicsRectItem(0,0,8,8,this);
        rectHandle->setBrush(QBrush(QColor(135, 206, 250)));
        rectHandle->setPen(QPen(QColor(135,206,250)));
        rectHandles.append(rectHandle);
    }

    updateRectHandles();
}

void selectItem::updateRectHandles(){
    screenshotView* screenshot = screenshotView::getInstance();
    QPoint selectStart = screenshot->getSelectStart();
    QPoint selectEnd = screenshot->getSelectEnd();
    QList<QRectF> handles = {
        QRectF(selectStart.x()-4,selectStart.y()-4,8,8),
        QRectF((selectStart.x()+selectEnd.x())/2-4,selectStart.y()-4,8,8),
        QRectF(selectEnd.x()-4,selectStart.y()-4,8,8),
        QRectF(selectEnd.x()-4,(selectStart.y()+selectEnd.y())/2-4,8,8),
        QRectF(selectEnd.x()-4,selectEnd.y()-4,8,8),
        QRectF((selectStart.x()+selectEnd.x())/2-4,selectEnd.y()-4,8,8),
        QRectF(selectStart.x()-4,selectEnd.y()-4,8,8),
        QRectF(selectStart.x()-4,(selectStart.y()+selectEnd.y())/2-4,8,8)
    };

    for(int i = 0;i < handles.size();i++){
        rectHandles[i]->setRect(handles[i]);
    }
}

QList<QGraphicsRectItem*> selectItem::getRectHandles(){
    return rectHandles;
}

void selectItem::setHover(bool setAccept){
    setAcceptHoverEvents(setAccept);
}

void selectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    screenshotView* screenshot = screenshotView::getInstance();
    QPoint selectStart = screenshot->getSelectStart();
    QPoint selectEnd = screenshot->getSelectEnd();
    QList<QRectF> handles = {
        QRectF(selectStart.x()-4,selectStart.y()-4,8,8),
        QRectF((selectStart.x()+selectEnd.x())/2-4,selectStart.y()-4,8,8),
        QRectF(selectEnd.x()-4,selectStart.y()-4,8,8),
        QRectF(selectEnd.x()-4,(selectStart.y()+selectEnd.y())/2-4,8,8),
        QRectF(selectEnd.x()-4,selectEnd.y()-4,8,8),
        QRectF((selectStart.x()+selectEnd.x())/2-4,selectEnd.y()-4,8,8),
        QRectF(selectStart.x()-4,selectEnd.y()-4,8,8),
        QRectF(selectStart.x()-4,(selectStart.y()+selectEnd.y())/2-4,8,8)
    };

    if(handles[0].contains(event->pos()) || handles[4].contains(event->pos())){
        setCursor(Qt::SizeFDiagCursor);
    }else if(handles[1].contains(event->pos()) || handles[5].contains(event->pos())){
        setCursor(Qt::SizeVerCursor);
    }else if(handles[2].contains(event->pos()) || handles[6].contains(event->pos())){
        setCursor(Qt::SizeBDiagCursor);
    }else if(handles[3].contains(event->pos()) || handles[7].contains(event->pos())){
        setCursor(Qt::SizeHorCursor);
    }else if(QRectF(selectStart,selectEnd).contains(event->pos()) && commandManager::getInstance()->dragTotally == true){
        setCursor(Qt::SizeAllCursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
}

changeSelectItemType selectItem::mousePointIn(QPointF pos){
    screenshotView* screenshot = screenshotView::getInstance();
    QPoint selectStart = screenshot->getSelectStart();
    QPoint selectEnd = screenshot->getSelectEnd();
    QList<QRectF> handles = {
        QRectF(selectStart.x()-4,selectStart.y()-4,8,8),
        QRectF((selectStart.x()+selectEnd.x())/2-4,selectStart.y()-4,8,8),
        QRectF(selectEnd.x()-4,selectStart.y()-4,8,8),
        QRectF(selectEnd.x()-4,(selectStart.y()+selectEnd.y())/2-4,8,8),
        QRectF(selectEnd.x()-4,selectEnd.y()-4,8,8),
        QRectF((selectStart.x()+selectEnd.x())/2-4,selectEnd.y()-4,8,8),
        QRectF(selectStart.x()-4,selectEnd.y()-4,8,8),
        QRectF(selectStart.x()-4,(selectStart.y()+selectEnd.y())/2-4,8,8)
    };

    if(handles[0].contains(pos)){
        return RECT_TOP_LEFT;
    }else if(handles[1].contains(pos)){
        return RECT_TOP;
    }else if(handles[2].contains(pos)){
        return RECT_TOP_RIGHT;
    }else if(handles[3].contains(pos)){
        return RECT_RIGHT;
    }else if(handles[4].contains(pos)){
        return RECT_BOTTOM_RIGHT;
    }else if(handles[5].contains(pos)){
        return RECT_BOTTOM;
    }else if(handles[6].contains(pos)){
        return RECT_BOTTOM_LEFT;
    }else if(handles[7].contains(pos)){
        return RECT_LEFT;
    }else if(QRectF(selectStart,selectEnd).contains(pos)){
        return RECT_IN;
    }else{
        return RECT_NO;
    }
}

void selectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    type = mousePointIn(event->pos());
    if(type != RECT_NO && type != RECT_IN){
        commandManager::getInstance()->disableDrawRect();
        commandManager::getInstance()->disableDrawEllipse();
        commandManager::getInstance()->disableDrawArrow();
        commandManager::getInstance()->disableDrawPen();
        commandManager::getInstance()->disableDrawText();
        commandManager::getInstance()->disableDrawNumber();
        isChaningArea = true;

        QList<myRectItem*> editedRectItems;
        foreach(QGraphicsItem* item,this->scene()->items()){
            if(myRectItem* editedRectItem = dynamic_cast<myRectItem*>(item)){
                if(editedRectItem->flags() & QGraphicsItem::ItemIsMovable){
                    editedRectItems.append(editedRectItem);
                }
            }
        }

        QList<myEllipseItem*> editedEllipseItems;
        foreach(QGraphicsItem* item,this->scene()->items()){
            if(myEllipseItem* editedEllipseItem = dynamic_cast<myEllipseItem*>(item)){
                if(editedEllipseItem->flags() & QGraphicsItem::ItemIsMovable){
                    editedEllipseItems.append(editedEllipseItem);
                }
            }
        }

        QList<myArrowItem*> editedArrowItems;
        foreach(QGraphicsItem* item,this->scene()->items()){
            if(myArrowItem* editedArrowItem = dynamic_cast<myArrowItem*>(item)){
                if(editedArrowItem->flags() & QGraphicsItem::ItemIsMovable){
                    editedArrowItems.append(editedArrowItem);
                }
            }
        }

        QList<myPenItem*> editedPenItems;
        foreach(QGraphicsItem* item,this->scene()->items()){
            if(myPenItem* editedPenItem = dynamic_cast<myPenItem*>(item)){
                if(editedPenItem->flags() & QGraphicsItem::ItemIsMovable){
                    editedPenItems.append(editedPenItem);
                }
            }
        }

        QList<myTextItem*> editedTextItems;
        foreach(QGraphicsItem* item,this->scene()->items()){
            if(myTextItem* editedTextItem = dynamic_cast<myTextItem*>(item)){
                if(editedTextItem->flags() & QGraphicsItem::ItemIsMovable){
                    editedTextItems.append(editedTextItem);
                }
            }
        }

        QList<myNumberItem*> editedNumberItems;
        foreach(QGraphicsItem* item,this->scene()->items()){
            if(myNumberItem* editedNumberItem = dynamic_cast<myNumberItem*>(item)){
                if(editedNumberItem->flags() & QGraphicsItem::ItemIsMovable){
                    editedNumberItems.append(editedNumberItem);
                }
            }
        }

        QList<myNumberTextItem*> editedNumberTextItems;
        foreach(QGraphicsItem* item,this->scene()->items()){
            if(myNumberTextItem* editedNumberTextItem = dynamic_cast<myNumberTextItem*>(item)){
                if(editedNumberTextItem->flags() & QGraphicsItem::ItemIsMovable){
                    editedNumberTextItems.append(editedNumberTextItem);
                }
            }
        }

        minX = 10000;
        minY = 10000;
        maxX = 0;
        maxY = 0;

        if(!editedRectItems.isEmpty()){
            foreach(myRectItem* editedRectItem,editedRectItems){
                QRectF editedRectItemRect = editedRectItem->mapToScene(editedRectItem->boundingRect()).boundingRect();
                if(editedRectItemRect.x() < minX){
                    minX = editedRectItemRect.x();
                }
                if(editedRectItemRect.y() < minY){
                    minY = editedRectItemRect.y();
                }
                if(editedRectItemRect.right() > maxX){
                    maxX = editedRectItemRect.right();
                }
                if(editedRectItemRect.bottom() > maxY){
                    maxY = editedRectItemRect.bottom();
                }
            }
        }
        if(!editedEllipseItems.isEmpty()){
            foreach(myEllipseItem* editedEllipseItem,editedEllipseItems){
                QRectF editedEllipseItemRect = editedEllipseItem->mapToScene(editedEllipseItem->boundingRect()).boundingRect();
                if(editedEllipseItemRect.x() < minX){
                    minX = editedEllipseItemRect.x();
                }
                if(editedEllipseItemRect.y() < minY){
                    minY = editedEllipseItemRect.y();
                }
                if(editedEllipseItemRect.right() > maxX){
                    maxX = editedEllipseItemRect.right();
                }
                if(editedEllipseItemRect.bottom() > maxY){
                    maxY = editedEllipseItemRect.bottom();
                }
            }
        }
        if(!editedArrowItems.isEmpty()){
            foreach(myArrowItem* editedArrowItem,editedArrowItems){
                QRectF sceneRect = editedArrowItem->mapToScene(editedArrowItem->boundingRect()).boundingRect();
                if(sceneRect.x() < minX){
                    minX = sceneRect.x();
                }
                if(sceneRect.y() < minY){
                    minY = sceneRect.y();
                }
                if((sceneRect.x()+sceneRect.width()) > maxX){
                    maxX = sceneRect.x()+sceneRect.width();
                }
                if((sceneRect.y()+sceneRect.height()) > maxY){
                    maxY = sceneRect.y()+sceneRect.height();
                }
            }
        }
        if(!editedPenItems.isEmpty()){
            foreach(myPenItem* editedPenItem,editedPenItems){
                QRectF sceneRect = editedPenItem->mapToScene(editedPenItem->boundingRect()).boundingRect();
                if(sceneRect.x() < minX){
                    minX = sceneRect.x();
                }
                if(sceneRect.y() < minY){
                    minY = sceneRect.y();
                }
                if((sceneRect.x()+sceneRect.width()) > maxX){
                    maxX = sceneRect.x()+sceneRect.width();
                }
                if((sceneRect.y()+sceneRect.height()) > maxY){
                    maxY = sceneRect.y() + sceneRect.height();
                }
            }
        }
        if(!editedTextItems.isEmpty()){
            foreach(myTextItem* editedTextItem,editedTextItems){
                QRectF textRect = editedTextItem->mapToScene(editedTextItem->boundingRect()).boundingRect();
                if(textRect.x() < minX){
                    minX = textRect.x();
                }
                if(textRect.y() < minY){
                    minY = textRect.y();
                }
                if(textRect.right() > maxX){
                    maxX = textRect.right();
                }
                if(textRect.bottom() > maxY){
                    maxY = textRect.bottom();
                }
            }
        }
        if(!editedNumberItems.isEmpty()){
            foreach(myNumberItem* editedNumberItem,editedNumberItems){
                QRectF numberRect = editedNumberItem->mapToScene(editedNumberItem->boundingRect()).boundingRect();
                if(numberRect.x() < minX){
                    minX = numberRect.x();
                }
                if(numberRect.y() < minY){
                    minY = numberRect.y();
                }
                if((numberRect.x()+numberRect.width()) > maxX){
                    maxX = numberRect.x()+numberRect.width();
                }
                if((numberRect.y()+numberRect.height()) > maxY){
                    maxY = numberRect.y() + numberRect.height();
                }
            }
        }
        if(!editedNumberTextItems.isEmpty()){
            foreach(myNumberTextItem* editedNumberTextItem,editedNumberTextItems){
                QRectF numberTextRect = editedNumberTextItem->mapToScene(editedNumberTextItem->boundingRect()).boundingRect();
                if(numberTextRect.x() < minX){
                    minX = numberTextRect.x();
                }
                if(numberTextRect.y() < minY){
                    minY = numberTextRect.y();
                }
                if((numberTextRect.x()+numberTextRect.width()) > maxX){
                    maxX = numberTextRect.x()+numberTextRect.width();
                }
                if((numberTextRect.y()+numberTextRect.height()) > maxY){
                    maxY = numberTextRect.y() + numberTextRect.height();
                }
            }
        }
    }
    if(type == RECT_IN && commandManager::getInstance()->dragTotally == true){
        dragTotallyStart = event->pos();
        beforeSelectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
    }
    scene()->clearSelection();
}

void selectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(finishSelect){
        redoManager::getInstance()->clear();
        screenshotView::getInstance()->getControl()->updateButtonStatu();
        QPoint oldSelectStart = screenshotView::getInstance()->getSelectStart();
        QPoint oldSelectEnd = screenshotView::getInstance()->getSelectEnd();
        if(type == RECT_TOP_LEFT){
            if(event->pos().x() > minX && event->pos().y() > minY){
                this->setPath(getShadowPath(QPoint(minX,minY),oldSelectEnd));
                screenshotView::getInstance()->setSelectStart(QPointF(minX,minY));
            }else if(event->pos().x() > minX){
                this->setPath(getShadowPath(QPoint(minX,event->pos().y()),oldSelectEnd));
                screenshotView::getInstance()->setSelectStart(QPointF(minX,event->pos().y()));
            }else if(event->pos().y() > minY){
                this->setPath(getShadowPath(QPoint(event->pos().x(),minY),oldSelectEnd));
                screenshotView::getInstance()->setSelectStart(QPointF(event->pos().x(),minY));
            }else{
                this->setPath(getShadowPath(QPoint(event->pos().toPoint()),oldSelectEnd));
                screenshotView::getInstance()->setSelectStart(event->pos());
            }
        }else if(type == RECT_TOP){
            if(event->pos().y() > minY){
                this->setPath(getShadowPath(QPoint(oldSelectStart.x(),minY),oldSelectEnd));
                screenshotView::getInstance()->setSelectStart(QPoint(oldSelectStart.x(),minY));
            }else{
                this->setPath(getShadowPath(QPoint(oldSelectStart.x(),event->pos().y()),oldSelectEnd));
                screenshotView::getInstance()->setSelectStart(QPoint(oldSelectStart.x(),event->pos().y()));
            }
        }else if(type == RECT_TOP_RIGHT){
            if(event->pos().x() < maxX && event->pos().y() > minY){
                this->setPath(getShadowPath(QPoint(oldSelectStart.x(),minY),QPoint(maxX,oldSelectEnd.y())));
                screenshotView::getInstance()->setSelectStart(QPoint(oldSelectStart.x(),minY));
                screenshotView::getInstance()->setSelectEnd(QPoint(maxX,oldSelectEnd.y()));
            }else if(event->pos().x() < maxX){
                this->setPath(getShadowPath(QPoint(oldSelectStart.x(),event->pos().y()),QPoint(maxX,oldSelectEnd.y())));
                screenshotView::getInstance()->setSelectStart(QPoint(oldSelectStart.x(),event->pos().y()));
                screenshotView::getInstance()->setSelectEnd(QPoint(maxX,oldSelectEnd.y()));
            }else if(event->pos().y() > minY){
                this->setPath(getShadowPath(QPoint(oldSelectStart.x(),minY),QPoint(event->pos().x(),oldSelectEnd.y())));
                screenshotView::getInstance()->setSelectStart(QPoint(oldSelectStart.x(),minY));
                screenshotView::getInstance()->setSelectEnd(QPoint(event->pos().x(),oldSelectEnd.y()));
            }else{
                this->setPath(getShadowPath(QPoint(oldSelectStart.x(),event->pos().y()),QPoint(event->pos().x(),oldSelectEnd.y())));
                screenshotView::getInstance()->setSelectStart(QPoint(oldSelectStart.x(),event->pos().y()));
                screenshotView::getInstance()->setSelectEnd(QPoint(event->pos().x(),oldSelectEnd.y()));
            }
        }else if(type == RECT_RIGHT){
            if(event->pos().x() < maxX){
                this->setPath(getShadowPath(oldSelectStart,QPoint(maxX,oldSelectEnd.y())));
                screenshotView::getInstance()->setSelectEnd(QPoint(maxX,oldSelectEnd.y()));
            }else{
                this->setPath(getShadowPath(oldSelectStart,QPoint(event->pos().x(),oldSelectEnd.y())));
                screenshotView::getInstance()->setSelectEnd(QPoint(event->pos().x(),oldSelectEnd.y()));
            }
        }else if(type == RECT_BOTTOM_RIGHT){
            if(event->pos().x() < maxX && event->pos().y() < maxY){
                this->setPath(getShadowPath(oldSelectStart,QPoint(maxX,maxY)));
                screenshotView::getInstance()->setSelectEnd(QPoint(maxX,maxY));
            }else if(event->pos().x() < maxX){
                this->setPath(getShadowPath(oldSelectStart,QPoint(maxX,event->pos().y())));
                screenshotView::getInstance()->setSelectEnd(QPoint(maxX,event->pos().y()));
            }else if(event->pos().y() < maxY){
                this->setPath(getShadowPath(oldSelectStart,QPoint(event->pos().x(),maxY)));
                screenshotView::getInstance()->setSelectEnd(QPoint(event->pos().x(),maxY));
            }else{
                this->setPath(getShadowPath(oldSelectStart,event->pos().toPoint()));
                screenshotView::getInstance()->setSelectEnd(event->pos());
            }
        }else if(type == RECT_BOTTOM){
            if(event->pos().y() < maxY){
                this->setPath(getShadowPath(oldSelectStart,QPoint(oldSelectEnd.x(),maxY)));
                screenshotView::getInstance()->setSelectEnd(QPoint(oldSelectEnd.x(),maxY));
            }else{
                this->setPath(getShadowPath(oldSelectStart,QPoint(oldSelectEnd.x(),event->pos().y())));
                screenshotView::getInstance()->setSelectEnd(QPoint(oldSelectEnd.x(),event->pos().y()));
            }
        }else if(type == RECT_BOTTOM_LEFT){
            if(event->pos().x() > minX && event->pos().y() < maxY){
                this->setPath(getShadowPath(QPoint(minX,oldSelectStart.y()),QPoint(oldSelectEnd.x(),maxY)));
                screenshotView::getInstance()->setSelectStart(QPoint(minX,oldSelectStart.y()));
                screenshotView::getInstance()->setSelectEnd(QPoint(oldSelectEnd.x(),maxY));
            }else if(event->pos().x() > minX){
                this->setPath(getShadowPath(QPoint(minX,oldSelectStart.y()),QPoint(oldSelectEnd.x(),event->pos().y())));
                screenshotView::getInstance()->setSelectStart(QPoint(minX,oldSelectStart.y()));
                screenshotView::getInstance()->setSelectEnd(QPoint(oldSelectEnd.x(),event->pos().y()));
            }else if(event->pos().y() < maxY){
                this->setPath(getShadowPath(QPoint(event->pos().x(),oldSelectStart.y()),QPoint(oldSelectEnd.x(),maxY)));
                screenshotView::getInstance()->setSelectStart(QPoint(event->pos().x(),oldSelectStart.y()));
                screenshotView::getInstance()->setSelectEnd(QPoint(oldSelectEnd.x(),maxY));
            }else{
                this->setPath(getShadowPath(QPoint(event->pos().x(),oldSelectStart.y()),QPoint(oldSelectEnd.x(),event->pos().y())));
                screenshotView::getInstance()->setSelectStart(QPoint(event->pos().x(),oldSelectStart.y()));
                screenshotView::getInstance()->setSelectEnd(QPoint(oldSelectEnd.x(),event->pos().y()));
            }
        }else if(type == RECT_LEFT){
            if(event->pos().x() > minX){
                this->setPath(getShadowPath(QPoint(minX,oldSelectStart.y()),oldSelectEnd));
                screenshotView::getInstance()->setSelectStart(QPoint(minX,oldSelectStart.y()));
            }else{
                this->setPath(getShadowPath(QPoint(event->pos().x(),oldSelectStart.y()),oldSelectEnd));
                screenshotView::getInstance()->setSelectStart(QPoint(event->pos().x(),oldSelectStart.y()));
            }
        }
        if(type == RECT_IN && commandManager::getInstance()->dragTotally == true){
            int y_offset = event->pos().y() - dragTotallyStart.y();
            int x_offset = event->pos().x() - dragTotallyStart.x();
            screenshotView::getInstance()->setSelectStart(QPoint(beforeSelectRect.x()+x_offset,beforeSelectRect.y()+y_offset));
            screenshotView::getInstance()->setSelectEnd(QPoint(beforeSelectRect.right()+x_offset,beforeSelectRect.bottom()+y_offset));
            this->setPath(getShadowPath(QPoint(beforeSelectRect.x()+x_offset,beforeSelectRect.y()+y_offset),QPoint(beforeSelectRect.right()+x_offset,beforeSelectRect.bottom()+y_offset)));
        }
        if((type != RECT_NO && type != RECT_IN) || (type == RECT_IN && commandManager::getInstance()->dragTotally == true)){
            updateRectHandles();
            screenshotView::getInstance()->getInfo()->updateInfoLabel();
            screenshotView::getInstance()->getControl()->updateButtonStatu();
            screenshotView::getInstance()->getControl()->hide();
            screenshotView::getInstance()->getControl()->myColorWidget->hide();
            screenshotView::getInstance()->getControl()->myTextWidget->hide();
        }
    }
}

QPainterPath selectItem::getShadowPath(QPoint selectStart,QPoint selectEnd){
    QPainterPath globalPath;
    globalPath.lineTo(QGuiApplication::primaryScreen()->geometry().width(),0);
    globalPath.lineTo(QGuiApplication::primaryScreen()->geometry().width(),QGuiApplication::primaryScreen()->geometry().height());
    globalPath.lineTo(0,QGuiApplication::primaryScreen()->geometry().height());
    globalPath.closeSubpath();
    QPainterPath shadowPath;
    shadowPath.moveTo(selectStart);
    shadowPath.lineTo(selectEnd.x(),selectStart.y());
    shadowPath.lineTo(selectEnd);
    shadowPath.lineTo(selectStart.x(),selectEnd.y());
    shadowPath.closeSubpath();
    return globalPath.subtracted(shadowPath);
}


void selectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if(finishSelect && isChaningArea){
        if(type != RECT_NO){
            isChaningArea = false;
            updateSelectPoints();
        }
    }
    if(type == RECT_IN && commandManager::getInstance()->dragTotally == true){
        QRectF selectRect = QRectF(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd()).normalized();
        int y_offset = event->pos().y() - dragTotallyStart.y();
        int x_offset = event->pos().x() - dragTotallyStart.x();
        if(selectRect.x() < 0 && selectRect.y() < 0){
            screenshotView::getInstance()->setSelectStart(QPoint(0,0));
            screenshotView::getInstance()->setSelectEnd(QPoint(selectRect.width(),selectRect.height()));
            this->setPath(getShadowPath(QPoint(0,0),QPoint(selectRect.width(),selectRect.height())));
        }else if(selectRect.x() < 0 && selectRect.bottom() > QGuiApplication::primaryScreen()->geometry().height()){
            screenshotView::getInstance()->setSelectStart(QPoint(0,QGuiApplication::primaryScreen()->geometry().height()-selectRect.height()));
            screenshotView::getInstance()->setSelectEnd(QPoint(selectRect.width(),QGuiApplication::primaryScreen()->geometry().height()));
            this->setPath(getShadowPath(QPoint(0,QGuiApplication::primaryScreen()->geometry().height()-selectRect.height()),QPoint(selectRect.width(),QGuiApplication::primaryScreen()->geometry().height())));
        }else if(selectRect.right() > QGuiApplication::primaryScreen()->geometry().width() && selectRect.y() < 0){
            screenshotView::getInstance()->setSelectStart(QPoint(QGuiApplication::primaryScreen()->geometry().width()-selectRect.width(),0));
            screenshotView::getInstance()->setSelectEnd(QPoint(QGuiApplication::primaryScreen()->geometry().width(),selectRect.height()));
            this->setPath(getShadowPath(QPoint(QGuiApplication::primaryScreen()->geometry().width()-selectRect.width(),0),QPoint(QGuiApplication::primaryScreen()->geometry().width(),selectRect.height())));
        }else if(selectRect.right() > QGuiApplication::primaryScreen()->geometry().width() && selectRect.bottom() > QGuiApplication::primaryScreen()->geometry().height()){
            screenshotView::getInstance()->setSelectStart(QPoint(QGuiApplication::primaryScreen()->geometry().width()-selectRect.width(),QGuiApplication::primaryScreen()->geometry().height()-selectRect.height()));
            screenshotView::getInstance()->setSelectEnd(QPoint(QGuiApplication::primaryScreen()->geometry().width(),QGuiApplication::primaryScreen()->geometry().height()));
            this->setPath(getShadowPath(QPoint(QGuiApplication::primaryScreen()->geometry().width()-selectRect.width(),QGuiApplication::primaryScreen()->geometry().height()-selectRect.height()),QPoint(QGuiApplication::primaryScreen()->geometry().width(),QGuiApplication::primaryScreen()->geometry().height())));
        }else if(selectRect.x() < 0){
            screenshotView::getInstance()->setSelectStart(QPoint(0,beforeSelectRect.y()+y_offset));
            screenshotView::getInstance()->setSelectEnd(QPoint(selectRect.width(),beforeSelectRect.y()+y_offset+selectRect.height()));
            this->setPath(getShadowPath(QPoint(0,beforeSelectRect.y()+y_offset),QPoint(selectRect.width(),beforeSelectRect.y()+y_offset+selectRect.height())));
        }else if(selectRect.y() < 0){
            screenshotView::getInstance()->setSelectStart(QPoint(beforeSelectRect.x()+x_offset,0));
            screenshotView::getInstance()->setSelectEnd(QPoint(beforeSelectRect.x()+x_offset+selectRect.width(),selectRect.height()));
            this->setPath(getShadowPath(QPoint(beforeSelectRect.x()+x_offset,0),QPoint(beforeSelectRect.x()+x_offset+selectRect.width(),selectRect.height())));
        }else if(selectRect.right() > QGuiApplication::primaryScreen()->geometry().width()){
            screenshotView::getInstance()->setSelectStart(QPoint(QGuiApplication::primaryScreen()->geometry().width()-selectRect.width(),beforeSelectRect.y()+y_offset));
            screenshotView::getInstance()->setSelectEnd(QPoint(QGuiApplication::primaryScreen()->geometry().width(),beforeSelectRect.y()+y_offset+selectRect.height()));
            this->setPath(getShadowPath(QPoint(QGuiApplication::primaryScreen()->geometry().width()-selectRect.width(),beforeSelectRect.y()+y_offset),QPoint(QGuiApplication::primaryScreen()->geometry().width(),beforeSelectRect.y()+y_offset+selectRect.height())));
        }else if(selectRect.bottom() > QGuiApplication::primaryScreen()->geometry().height()){
            screenshotView::getInstance()->setSelectStart(QPoint(beforeSelectRect.x()+x_offset,QGuiApplication::primaryScreen()->geometry().height()-selectRect.height()));
            screenshotView::getInstance()->setSelectEnd(QPoint(beforeSelectRect.x()+x_offset+selectRect.width(),QGuiApplication::primaryScreen()->geometry().height()));
            this->setPath(getShadowPath(QPoint(beforeSelectRect.x()+x_offset,QGuiApplication::primaryScreen()->geometry().height()-selectRect.height()),QPoint(beforeSelectRect.x()+x_offset+selectRect.width(),QGuiApplication::primaryScreen()->geometry().height())));
        }
        updateRectHandles();
        screenshotView::getInstance()->getInfo()->updateInfoLabel();
    }
    screenshotView::getInstance()->getControl()->setLocation(screenshotView::getInstance()->getSelectStart(),screenshotView::getInstance()->getSelectEnd());
    screenshotView::getInstance()->getControl()->show();
}

void selectItem::setFinishSelect(){
    finishSelect = true;
}

void selectItem::updateSelectPoints(){
    QPointF selectStart = screenshotView::getInstance()->getSelectStart();
    QPointF selectEnd = screenshotView::getInstance()->getSelectEnd();
    if(selectEnd.x() < selectStart.x() && selectEnd.y() < selectStart.y()){
        screenshotView::getInstance()->setSelectStart(selectEnd);
        screenshotView::getInstance()->setSelectEnd(selectStart);
    }else if(selectEnd.x() < selectStart.x()){
        screenshotView::getInstance()->setSelectStart(QPointF(selectEnd.x(),selectStart.y()));
        screenshotView::getInstance()->setSelectEnd(QPointF(selectStart.x(),selectEnd.y()));
    }else if(selectEnd.y() < selectStart.y()){
        screenshotView::getInstance()->setSelectStart(QPointF(selectStart.x(),selectEnd.y()));
        screenshotView::getInstance()->setSelectEnd(QPointF(selectEnd.x(),selectStart.y()));
    }
}

void selectItem::hideRectHandles(){
    for(int i = 0;i < rectHandles.size();i++){
        rectHandles[i]->hide();
    }
}

















