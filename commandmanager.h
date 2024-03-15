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
    bool isEditingItem();
    void setEditingItem(bool editingItem);

    void connectToControlWidget();

    QPoint drawRectStart;
    QPoint drawRectEnd;
    QPoint drawEllipseStart;
    QPoint drawEllipseEnd;
    QPen rectPen;
    QPen ellipsePen;

private:
    static commandManager* instance;
    explicit commandManager(QObject *parent = nullptr);
    commandManager(const commandManager& other) = delete;
    commandManager& operator=(const commandManager& other) = delete;

    bool selectingArea = true;
    bool drawingRect = false;
    bool editingItem = false;
    bool drawingEllipse = false;

public slots:
    void enableDrawRect();
    void disableDrawRect();
    void enableDrawEllipse();
    void disableDrawEllipse();
    void quit();
};

#endif // COMMANDMANAGER_H
