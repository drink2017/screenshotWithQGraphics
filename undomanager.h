#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include "order.h"

#include <QQueue>

class undoManager
{
public:
    static undoManager* getInstance();
    void pushOrder(order* aOrder);
    order* popOrder();
    bool isEmpty();

    //void clear();

private:
    static undoManager* instance;

    undoManager();

    QQueue<order*> undo;
};

#endif // UNDOMANAGER_H
