#ifndef SELECTITEM_H
#define SELECTITEM_H

#include "QGraphicsPathItem"

enum changeSelectItemType{
    RECT_TOP_LEFT,
    RECT_TOP,
    RECT_TOP_RIGHT,
    RECT_RIGHT,
    RECT_BOTTOM_RIGHT,
    RECT_BOTTOM,
    RECT_BOTTOM_LEFT,
    RECT_LEFT,
    RECT_NO
};

class selectItem : public QGraphicsPathItem
{
public:
    selectItem();
    void updateRectHandles();
    QList<QGraphicsRectItem*> getRectHandles();
    void setHover(bool setAccept);

    void setFinishSelect();

private:
    QList<QGraphicsRectItem*> rectHandles;

    void createRectHandles();

    changeSelectItemType type;
    changeSelectItemType mousePointIn(QPointF pos);

    QPainterPath getShadowPath(QPoint selectStart,QPoint selectEnd);

    bool finishSelect = false;
    bool isChaningArea = false;

    void updateSelectPoints();

    int minX;
    int minY;
    int maxX;
    int maxY;

    // QGraphicsItem interface
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // SELECTITEM_H
