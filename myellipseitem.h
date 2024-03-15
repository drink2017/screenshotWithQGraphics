#ifndef MYELLIPSEITEM_H
#define MYELLIPSEITEM_H

#include <QGraphicsEllipseItem>

class myEllipseItem : public QGraphicsEllipseItem
{
public:
    myEllipseItem();
    myEllipseItem(const QRectF& rect,QGraphicsItem* parent = nullptr);
};

#endif // MYELLIPSEITEM_H
