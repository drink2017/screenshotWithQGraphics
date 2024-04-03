#ifndef MYNUMBERITEM_H
#define MYNUMBERITEM_H

#include <QGraphicsItemGroup>

class myNumberItem : public QGraphicsItemGroup
{
public:
    myNumberItem(int number,QGraphicsItem* parent = nullptr);

    void setColor(QColor color);

private:
    QGraphicsEllipseItem* ellipse;
    QGraphicsSimpleTextItem* serial;
    QGraphicsTextItem* text;

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MYNUMBERITEM_H
