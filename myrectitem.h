#ifndef MYRECTITEM_H
#define MYRECTITEM_H

#include <QGraphicsRectItem>

enum pointIn{
    rect_top_left,
    rect_top,
    rect_top_right,
    rect_right,
    rect_bottom_right,
    rect_bottom,
    rect_bottom_left,
    rect_left,
    rect_no
};

class myRectItem : public QGraphicsRectItem
{
public:
    myRectItem(QGraphicsItem* parent = nullptr);
    myRectItem(const QRectF& rect, QGraphicsItem* parent = nullptr);

    void updateEllipseHandles();

    void setNowRect(QRectF nowRect);
    QRectF getNowRect();

private:
    QList<QGraphicsEllipseItem*> ellipseHandles;

    void createEllipseHandles();
    void showEllipseHandles();
    void hideEllipseHandles();

    bool isMouseOnBoundary(const QPointF& pos) const;

    pointIn mousePointIn(QPointF pos);
    pointIn type;
    QRectF before;

    QRectF nowRect;

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // QGraphicsItem interface
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MYRECTITEM_H
