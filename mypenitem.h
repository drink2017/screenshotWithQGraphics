#ifndef MYPENITEM_H
#define MYPENITEM_H

# include "QGraphicsPathItem"

class myPenItem : public QGraphicsPathItem
{
public:
    myPenItem(QPainterPath path,QGraphicsItem* parent = nullptr);

private:
    bool isMouseOnBoundary(const QPointF& pos) const;

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // MYPENITEM_H
