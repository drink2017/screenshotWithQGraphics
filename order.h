#ifndef ORDER_H
#define ORDER_H

#include <QGraphicsItem>
#include <QQueue>

class order
{
public:
    order();
    void addToAddItem(QGraphicsItem* item);
    void addToDeleteItem(QGraphicsItem* item);

    QQueue<QGraphicsItem*> getAddItem();
    QQueue<QGraphicsItem*> getDeleteItem();

    void clearAddItem();
    void clearDeleteItem();

private:
    QQueue<QGraphicsItem*> addItem;
    QQueue<QGraphicsItem*> deleteItem;
};

#endif // ORDER_H
