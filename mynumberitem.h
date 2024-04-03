#ifndef MYNUMBERITEM_H
#define MYNUMBERITEM_H

#include "mynumbertextitem.h"

#include <QGraphicsItemGroup>

class myNumberItem : public QGraphicsItemGroup
{
public:
    myNumberItem(int number,QGraphicsItem* parent = nullptr);

    void setColor(QColor color);

    QGraphicsEllipseItem* ellipse;
    QGraphicsSimpleTextItem* serial;
    myNumberTextItem* text;

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MYNUMBERITEM_H
