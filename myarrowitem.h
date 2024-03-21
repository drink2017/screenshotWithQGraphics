#ifndef MYARROWITEM_H
#define MYARROWITEM_H

#include <QGraphicsItem>
#include <QPen>

class myArrowItem : public QGraphicsItem
{
public:
    myArrowItem();
    myArrowItem(QPointF start_pos, QPointF end_pos,QGraphicsItem* parent = nullptr);

    void setPen(QPen commandPen);

    void setStart(QPoint startPoint);
    void setEnd(QPoint endPoint);

private:
    QPointF start_pos;
    QPointF end_pos;

    QPen arrowPen;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;
};

#endif // MYARROWITEM_H
