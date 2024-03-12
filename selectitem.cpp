#include "selectitem.h"
#include "screenshotview.h"
#include "commandmanager.h"

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
    }
    QList<QGraphicsItem*> selectedItems = screenshotView::getInstance()->getScene()->selectedItems();
    if (!selectedItems.isEmpty()) {
        foreach (QGraphicsItem* item, selectedItems) {
            item->setSelected(false);
        }
    }
}

void selectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(finishSelect){
        isChaningArea = true;
        QPoint oldSelectStart = screenshotView::getInstance()->getSelectStart();
        QPoint oldSelectEnd = screenshotView::getInstance()->getSelectEnd();
        if(type == RECT_TOP_LEFT){
            this->setPath(getShadowPath(QPoint(event->pos().toPoint()),oldSelectEnd));
            screenshotView::getInstance()->setSelectStart(event->pos());
        }else if(type == RECT_TOP){
            this->setPath(getShadowPath(QPoint(oldSelectStart.x(),event->pos().y()),oldSelectEnd));
            screenshotView::getInstance()->setSelectStart(QPoint(oldSelectStart.x(),event->pos().y()));
        }else if(type == RECT_TOP_RIGHT){
            this->setPath(getShadowPath(QPoint(oldSelectStart.x(),event->pos().y()),QPoint(event->pos().x(),oldSelectEnd.y())));
            screenshotView::getInstance()->setSelectStart(QPoint(oldSelectStart.x(),event->pos().y()));
            screenshotView::getInstance()->setSelectEnd(QPoint(event->pos().x(),oldSelectEnd.y()));
        }else if(type == RECT_RIGHT){
            this->setPath(getShadowPath(oldSelectStart,QPoint(event->pos().x(),oldSelectEnd.y())));
            screenshotView::getInstance()->setSelectEnd(QPoint(event->pos().x(),oldSelectEnd.y()));
        }else if(type == RECT_BOTTOM_RIGHT){
            this->setPath(getShadowPath(oldSelectStart,event->pos().toPoint()));
            screenshotView::getInstance()->setSelectEnd(event->pos());
        }else if(type == RECT_BOTTOM){
            this->setPath(getShadowPath(oldSelectStart,QPoint(oldSelectEnd.x(),event->pos().y())));
            screenshotView::getInstance()->setSelectEnd(QPoint(oldSelectEnd.x(),event->pos().y()));
        }else if(type == RECT_BOTTOM_LEFT){
            this->setPath(getShadowPath(QPoint(event->pos().x(),oldSelectStart.y()),QPoint(oldSelectEnd.x(),event->pos().y())));
            screenshotView::getInstance()->setSelectStart(QPoint(event->pos().x(),oldSelectStart.y()));
            screenshotView::getInstance()->setSelectEnd(QPoint(oldSelectEnd.x(),event->pos().y()));
        }else if(type == RECT_LEFT){
            this->setPath(getShadowPath(QPoint(event->pos().x(),oldSelectStart.y()),oldSelectEnd));
            screenshotView::getInstance()->setSelectStart(QPoint(event->pos().x(),oldSelectStart.y()));
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


















