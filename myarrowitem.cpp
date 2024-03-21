#include "myarrowitem.h"
#include "screenshotview.h"

#include <QPainter>
#include <cmath>

myArrowItem::myArrowItem()
{

}

myArrowItem::myArrowItem(QPointF start_pos, QPointF end_pos,QGraphicsItem* parent):QGraphicsItem(parent),start_pos(start_pos),end_pos(end_pos){

}


QRectF myArrowItem::boundingRect() const
{
    return shape().boundingRect();
}

void myArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(arrowPen);
    painter->drawLine(start_pos,end_pos);

    // 绘制箭头头部
    QPointF arrowHead[3];
    qreal angle = std::atan2((end_pos.y() - start_pos.y()), (end_pos.x() - start_pos.x()));
    arrowHead[0] = end_pos;
    arrowHead[1] = end_pos - QPointF(sin(angle + M_PI / 3) * 10, cos(angle + M_PI / 3) * 10);
    arrowHead[2] = end_pos - QPointF(sin(angle + M_PI - M_PI / 3) * 10, cos(angle + M_PI - M_PI / 3) * 10);
    painter->drawPolygon(arrowHead, 3);
}

QPainterPath myArrowItem::shape() const
{
    QPainterPath path;
    path.moveTo(start_pos);
    path.lineTo(end_pos);
    return path;
}

void myArrowItem::setPen(QPen commandPen){
    arrowPen.setWidth(commandPen.width());
    arrowPen.setColor(commandPen.color());
}

void myArrowItem::setStart(QPoint startPoint){
    this->start_pos = startPoint;
    this->scene()->update();
}

void myArrowItem::setEnd(QPoint endPoint){
    this->end_pos = endPoint;
    this->scene()->update();
}
