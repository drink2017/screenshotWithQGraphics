#ifndef REDOMANAGER_H
#define REDOMANAGER_H

#include "order.h"

#include <QQueue>

class redoManager
{
public:
    static redoManager* getInstance();

    void pushOrder(order* aOrder);
    order* popOrder();

    bool isEmpty();

    void clear();

private:
    static redoManager* instance;

    redoManager();

    QQueue<order*> redo;
};

#endif // REDOMANAGER_H
