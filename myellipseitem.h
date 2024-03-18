#ifndef MYELLIPSEITEM_H
#define MYELLIPSEITEM_H

#include <QGraphicsEllipseItem>

enum pointInEllipse{
    ellipse_rect_top,
    ellipse_rect_right,
    ellipse_rect_bottom,
    ellipse_rect_left,
    ellipse_rect_no
};

class myEllipseItem : public QGraphicsEllipseItem
{
public:
    myEllipseItem();
    myEllipseItem(const QRectF& rect,QGraphicsItem* parent = nullptr);

    void updateEllipseHandles();

private:
    void createEllipseHandles();
    void showEllipseHandles();
    void hideEllipseHandles();
    QList<QGraphicsEllipseItem*> ellipseHandles;

    pointInEllipse mousePointIn(QPointF pos);
    pointInEllipse type;

    //用于编辑item
    QRectF before;

    //用于undo,redo
    //QRectF nowRect;

    bool isMouseOnBoundary(const QPointF& pos) const;

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MYELLIPSEITEM_H
