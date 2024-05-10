#ifndef MYTEXTITEM_H
#define MYTEXTITEM_H

#include <QGraphicsTextItem>

class myTextItem : public QGraphicsTextItem
{
public:
    myTextItem(QGraphicsItem* parent = nullptr);

    // QGraphicsItem interface
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

public slots:
    void adjustTextWidth();

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // MYTEXTITEM_H
