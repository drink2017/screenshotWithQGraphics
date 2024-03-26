#ifndef MYPENITEM_H
#define MYPENITEM_H

# include "QGraphicsPathItem"

class myPenItem : public QGraphicsPathItem
{
public:
    myPenItem(QPainterPath path,QGraphicsItem* parent = nullptr);
};

#endif // MYPENITEM_H
