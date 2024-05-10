#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include "colorwidget.h"
#include "settings.h"
#include "textwidget.h"

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

class controlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit controlWidget(QWidget *parent = nullptr);

    colorWidget* myColorWidget;
    textWidget* myTextWidget;

    QRect getLocationRect();

    void setLocation(QPoint selectStart,QPoint selectEnd);

    void updateButtonStatu();

signals:

private:
    void setButtons();

    //十个选项按钮
    QPushButton* pRectButton;
    QPushButton* pRoundButton;
    QPushButton* pArrowButton;
    QPushButton* pPenButton;
    QPushButton* pTextButton;
    QPushButton* pSerialButton;
    QPushButton* pUndoButton;
    QPushButton* pRedoButton;
    QPushButton* pOKButton;
    QPushButton* pNOButton;

    //布局
    QHBoxLayout* pLayout;

    void addButtonsToLayout();

    QRect locationRect;

    void connectToSlots();

public slots:
    void rectButtonStatu();
    void roundButtonStatu();
    void arrowButtonStatu();
    void penButtonStatu();
    void textButtonStatu();
    void numberButtonStatu();
    void undo();
    void redo();
    void shot();

signals:
    void enableDrawRect();
    void disableDrawRect();
    void enableDrawRound();
    void disableDrawRound();
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

#endif // CONTROLWIDGET_H
