#ifndef MYNUMBERTEXTITEM_H
#define MYNUMBERTEXTITEM_H

#include <QGraphicsTextItem>

class myNumberTextItem : public QGraphicsTextItem
{
public:
    myNumberTextItem();

public slots:
    void adjustTextWidth();

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MYNUMBERTEXTITEM_H
