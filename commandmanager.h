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
    bool isDrawingText();
    void setDrawingText(bool drawingText);
    bool isDrawingNumber();
    void setDrawingNumber(bool drawingNumber);
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

    int number = 1;

    bool dragTotally = true;

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
    bool drawingText = false;
    bool drawingNumber = false;

public slots:
    void enableDrawRect();
    void disableDrawRect();
    void enableDrawEllipse();
    void disableDrawEllipse();
    void enableDrawArrow();
    void disableDrawArrow();
    void enableDrawPen();
    void disableDrawPen();
    void enableDrawText();
    void disableDrawText();
    void enableDrawNumber();
    void disableDrawNumber();
    void quit();
};

#endif // COMMANDMANAGER_H
