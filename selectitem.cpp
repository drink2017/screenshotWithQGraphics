#include "selectitem.h"
#include "screenshotview.h"
#include "commandmanager.h"
#include "myrectitem.h"
#include "myellipseitem.h"
#include "myarrowitem.h"

#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

selectItem::selectItem()
{
    setPen(QPen(Qt::blue));
    setBrush(QBrush(QColor(0,0,0,100)));
    createRectHandles();
}

void selectItem::createRectHandles(){
    for(int i = 0;i < 8;i++){
        QGraphicsRectItem* rectHandle = new QGraphicsRectItem(0,0,10,10,this);
        rectHandle->setBrush(QBrush(Qt::blue));
        rectHandles.append(rectHandle);
    }

    updateRectHandles();
}

void selectItem::updateRectHandles(){
    screenshotView* screenshot = screenshotView::getInstance();
    QPoint selectStart = screenshot->getSelectStart();
    QPoint selectEnd = screenshot->getSelectEnd();
    QList<QRectF> handles = {
        QRectF(selectStart.x()-5,selectStart.y()-5,10,10),
        QRectF((selectStart.x()+selectEnd.x())/2-5,selectStart.y()-5,10,10),
        QRectF(selectEnd.x()-5,selectStart.y()-5,10,10),
        QRectF(selectEnd.x()-5,(selectStart.y()+selectEnd.y())/2-5,10,10),
        QRectF(selectEnd.x()-5,selectEnd.y()-5,10,10),
        QRectF((selectStart.x()+selectEnd.x())/2-5,selectEnd.y()-5,10,10),
        QRectF(selectStart.x()-5,selectEnd.y()-5,10,10),
        QRectF(selectStart.x()-5,(selectStart.y()+selectEnd.y())/2-5,10,10)
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
        QRectF(selectStart.x()-5,selectStart.y()-5,10,10),
        QRectF((selectStart.x()+selectEnd.x())/2-5,selectStart.y()-5,10,10),
        QRectF(selectEnd.x()-5,selectStart.y()-5,10,10),
        QRectF(selectEnd.x()-5,(selectStart.y()+selectEnd.y())/2-5,10,10),
        QRectF(selectEnd.x()-5,selectEnd.y()-5,10,10),
        QRectF((selectStart.x()+selectEnd.x())/2-5,selectEnd.y()-5,10,10),
        QRectF(selectStart.x()-5,selectEnd.y()-5,10,10),
        QRectF(selectStart.x()-5,(selectStart.y()+selectEnd.y())/2-5,10,10)
    };

    if(handles[0].contains(event->pos()) || handles[4].contains(event->pos())){
        setCursor(Qt::SizeFDiagCursor);
    }else if(handles[1].contains(event->pos()) || handles[5].contains(event->pos())){
        setCursor(Qt::SizeVerCursor);
    }else if(handles[2].contains(event->pos()) || handles[6].contains(event->pos())){
        setCursor(Qt::SizeBDiagCursor);
    }else if(handles[3].contains(event->pos()) || handles[7].contains(event->pos())){
        setCursor(Qt::SizeHorCursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
}

changeSelectItemType selectItem::mousePointIn(QPointF pos){
    screenshotView* screenshot = screenshotView::getInstance();
    QPoint selectStart = screenshot->getSelectStart();
    QPoint selectEnd = screenshot->getSelectEnd();
    QList<QRectF> handles = {
        QRectF(selectStart.x()-5,selectStart.y()-5,10,10),
        QRectF((selectStart.x()+selectEnd.x())/2-5,selectStart.y()-5,10,10),
        QRectF(selectEnd.x()-5,selectStart.y()-5,10,10),
        QRectF(selectEnd.x()-5,(selectStart.y()+selectEnd.y())/2-5,10,10),
        QRectF(selectEnd.x()-5,selectEnd.y()-5,10,10),
        QRectF((selectStart.x()+selectEnd.x())/2-5,selectEnd.y()-5,10,10),
        QRectF(selectStart.x()-5,selectEnd.y()-5,10,10),
        QRectF(selectStart.x()-5,(selectStart.y()+selectEnd.y())/2-5,10,10)
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
    }else{
        return RECT_NO;
    }
}

void selectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    type = mousePointIn(event->pos());
    if(type != RECT_NO){
        commandManager::getInstance()->disableDrawRect();
        commandManager::getInstance()->disableDrawEllipse();
        commandManager::getInstance()->disableDrawArrow();
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

        minX = 10000;
        minY = 10000;
        maxX = 0;
        maxY = 0;

        if(!editedRectItems.isEmpty()){
            foreach(myRectItem* editedRectItem,editedRectItems){
                if(editedRectItem->getNowRect().x() < minX){
                    minX = editedRectItem->getNowRect().x();
                }
                if(editedRectItem->getNowRect().y() < minY){
                    minY = editedRectItem->getNowRect().y();
                }
                if((editedRectItem->getNowRect().x()+editedRectItem->getNowRect().width()) > maxX){
                    maxX = editedRectItem->getNowRect().x()+editedRectItem->getNowRect().width();
                }
                if((editedRectItem->getNowRect().y()+editedRectItem->getNowRect().height()) > maxY){
                    maxY = editedRectItem->getNowRect().y()+editedRectItem->getNowRect().height();
                }
            }
        }
        if(!editedEllipseItems.isEmpty()){
            foreach(myEllipseItem* editedEllipseItem,editedEllipseItems){
                if(editedEllipseItem->getNowRect().x() < minX){
                    minX = editedEllipseItem->getNowRect().x();
                }
                if(editedEllipseItem->getNowRect().y() < minY){
                    minY = editedEllipseItem->getNowRect().y();
                }
                if((editedEllipseItem->getNowRect().x()+editedEllipseItem->getNowRect().width()) > maxX){
                    maxX = editedEllipseItem->getNowRect().x()+editedEllipseItem->getNowRect().width();
                }
                if((editedEllipseItem->getNowRect().y()+editedEllipseItem->getNowRect().height()) > maxY){
                    maxY = editedEllipseItem->getNowRect().y()+editedEllipseItem->getNowRect().height();
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
    }
    scene()->clearSelection();
}

void selectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(finishSelect){
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
        if(type != RECT_NO){
            updateRectHandles();
            screenshotView::getInstance()->getInfo()->updateInfoLabel();
            screenshotView::getInstance()->getControl()->updateButtonStatu();
            screenshotView::getInstance()->getControl()->hide();
            screenshotView::getInstance()->getControl()->myColorWidget->hide();
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


















