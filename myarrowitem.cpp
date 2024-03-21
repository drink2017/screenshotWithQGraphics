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

    arrowPen.setCapStyle(Qt::RoundCap);
    painter->setPen(arrowPen);

    painter->drawLine(start_pos,end_pos);

    int size = painter->pen().width() * 10;

    double angle = std::atan2(-1.0 * (end_pos.y() - start_pos.y()), end_pos.x() - start_pos.x());
    QPoint arrowP1 = end_pos.toPoint() + QPoint(-1 * size * std::cos(angle - M_PI / 6), size * std::sin(angle - M_PI / 6));
    QPoint arrowP2 = end_pos.toPoint() + QPoint(-1 * size * std::cos(angle + M_PI / 6), size * std::sin(angle + M_PI / 6));

    // 画箭头尖端
    QPainterPath path;
    path.moveTo(end_pos);
    path.lineTo(arrowP1);
    path.lineTo(arrowP2);
    path.closeSubpath();

    // 填充箭头尖端
    painter->fillPath(path, painter->pen().color());
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
