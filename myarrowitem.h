#ifndef MYARROWITEM_H
#define MYARROWITEM_H

#include <QGraphicsItem>
#include <QPen>

enum pointInArrow{
    arrow_start,
    arrow_end,
    arrow_no
};

class myArrowItem : public QGraphicsItem
{
public:
    myArrowItem();
    myArrowItem(QPointF start_pos, QPointF end_pos,QGraphicsItem* parent = nullptr);

    void setPen(QPen commandPen);

    void setStart(QPoint startPoint);
    void setEnd(QPoint endPoint);

    void updateEllipseHandles();

private:
    QPointF start_pos;
    QPointF end_pos;

    QPen arrowPen;

    QList<QGraphicsEllipseItem*> ellipseHandles;
    void createEllipseHandles();
    void showEllipseHandles();
    void hideEllipseHandles();

    pointInArrow type;
    pointInArrow mousePointIn(QPointF pos);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

    // QGraphicsItem interface
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MYARROWITEM_H
