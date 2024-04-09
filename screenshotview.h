#ifndef SCREENSHOTVIEW_H
#define SCREENSHOTVIEW_H

#include "selectitem.h"
#include "infolabel.h"
#include "controlwidget.h"
#include "myarrowitem.h"
#include "command.h"

#include <QGraphicsView>

class screenshotView : public QGraphicsView
{
    Q_OBJECT
public:
    static screenshotView* getInstance();

    QPoint getSelectStart();
    QPoint getSelectEnd();

    controlWidget* getControl();
    void setControl(controlWidget* control);

    infoLabel* getInfo();
    void setInfo(infoLabel* info);

    QGraphicsScene* getScene();

    void setSelectStart(QPointF selectStart);
    void setSelectEnd(QPointF selectEnd);
    QPainterPath getShadowPath(QPoint selectStart,QPoint selectEnd);

    void setCurrentSelectItem(selectItem* selectArea);
    selectItem* getCurrentSelectItem();
    QGraphicsRectItem* getCurrentRectItem();
    QGraphicsEllipseItem* getCurrentEllipseItem();
    myArrowItem* getCurrentArrowItem();
    QGraphicsPathItem* getCurrentPenItem();

private:
    static screenshotView* instance;

    screenshotView();
    screenshotView(const screenshotView& other) = delete;
    screenshotView& operator=(const screenshotView& other) = delete;

    QGraphicsScene* scene;

    //选择区域
    QPoint selectStart;
    QPoint selectEnd;
    selectItem* currentSelectItem;

    //显示尺寸区域
    infoLabel* info;

    //控制栏区域
    controlWidget* control;

    //要画的图元
    QGraphicsRectItem* currentRectItem = new QGraphicsRectItem(QRectF());
    QGraphicsEllipseItem* currentEllipseItem = new QGraphicsEllipseItem(QRectF());
    myArrowItem* currentArrowItem = new myArrowItem(QPointF(),QPointF());
    QGraphicsPathItem* currentPenItem = new QGraphicsPathItem();

    bool focusOnText;
    bool focusOnNumber;

    command* pCommand;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // SCREENSHOTVIEW_H
