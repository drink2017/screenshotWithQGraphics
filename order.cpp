#include "order.h"

order::order()
{

}

void order::addToAddItem(QGraphicsItem *item){
    addItem.enqueue(item);
}

void order::addToDeleteItem(QGraphicsItem *item){
    deleteItem.enqueue(item);
}

QQueue<QGraphicsItem*> order::getAddItem(){
    return addItem;
}

QQueue<QGraphicsItem*> order::getDeleteItem(){
    return deleteItem;
}
