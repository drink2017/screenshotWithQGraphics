#ifndef SCREENSHOTVIEW_H
#define SCREENSHOTVIEW_H

#include "selectitem.h"
#include "infolabel.h"
#include "controlwidget.h"

#include <QGraphicsView>

class screenshotView : public QGraphicsView
{
    Q_OBJECT
public:
    static screenshotView* getInstance();

    QPoint getSelectStart();
    QPoint getSelectEnd();
    controlWidget* getControl();
    infoLabel* getInfo();
    QGraphicsScene* getScene();

    void setSelectStart(QPointF selectStart);
    void setSelectEnd(QPointF selectEnd);

private:
    static screenshotView* instance;

    screenshotView();
    screenshotView(const screenshotView& other) = delete;
    screenshotView& operator=(const screenshotView& other) = delete;

    QGraphicsScene* scene;

    //选择区域
    QPoint selectStart;
    QPoint selectEnd;
    QPainterPath getShadowPath(QPoint selectStart,QPoint selectEnd);
    selectItem* currentSelectItem;

    //显示尺寸区域
    infoLabel* info;

    //控制栏区域
    controlWidget* control;

    //要画的图元
    QGraphicsRectItem* currentRectItem = new QGraphicsRectItem(QRectF());

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // SCREENSHOTVIEW_H
