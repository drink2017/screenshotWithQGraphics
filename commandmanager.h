#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QPoint>
#include <QPen>

class commandManager : public QObject
{
    Q_OBJECT
public:
    static commandManager* getInstance();

    bool isSelectingArea();
    void setSelectingArea(bool selectingArea);
    bool isDrawingRect();
    void setDrawingRect(bool drawingRect);
    bool isDrawingEllipse();
    void setDrawingEllipse(bool drawingEllipse);
    bool isDrawingArrow();
    void setDrawingArrow(bool drawingArrow);
    bool isDrawingPen();
    void setDrawingPen(bool drawingPen);
    bool isEditingItem();
    void setEditingItem(bool editingItem);

    void connectToControlWidget();

    QPoint drawRectStart;
    QPoint drawRectEnd;
    QPoint drawEllipseStart;
    QPoint drawEllipseEnd;
    QPoint drawArrowStart;
    QPoint drawArrowEnd;
    QPen rectPen;
    QPen ellipsePen;
    QPen arrowPen;
    QPen penPen;

private:
    static commandManager* instance;
    explicit commandManager(QObject *parent = nullptr);
    commandManager(const commandManager& other) = delete;
    commandManager& operator=(const commandManager& other) = delete;

    bool selectingArea = true;
    bool drawingRect = false;
    bool editingItem = false;
    bool drawingEllipse = false;
    bool drawingArrow = false;
    bool drawingPen = false;

public slots:
    void enableDrawRect();
    void disableDrawRect();
    void enableDrawEllipse();
    void disableDrawEllipse();
    void enableDrawArrow();
    void disableDrawArrow();
    void enableDrawPen();
    void disableDrawPen();
    void quit();
};

#endif // COMMANDMANAGER_H
