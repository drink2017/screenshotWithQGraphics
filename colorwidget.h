#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "settings.h"

enum widgetType{
    rect,
    ellipse,
    arrow,
    pen
};

class colorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit colorWidget(QWidget *parent = nullptr);

    Settings* settings;

    void setType(widgetType type);

    void setLocationBelow();
    void setLocationAbove();
    void show();

signals:

private:
    void setButtons();
    void addWidgetToLayout();
    void setInitialButton();
    void setInitialLabel();

    //布局
    QHBoxLayout* totalLayout;
    QHBoxLayout* colorLayout1;
    QHBoxLayout* colorLayout2;
    QVBoxLayout* vColorLayout;

    //粗细按钮
    QPushButton* xiButton;
    QPushButton* zhongButton;
    QPushButton* cuButton;

    //显示颜色的标签
    QLabel* choice;

    //16个颜色按钮
    QPushButton* blackButton;
    QPushButton* darkGrayButton;
    QPushButton* darkRedButton;
    QPushButton* orangeButton;
    QPushButton* darkGreenButton;
    QPushButton* darkBlueButton;
    QPushButton* darkMagentaButton;
    QPushButton* darkCyanButton;

    QPushButton* whiteButton;
    QPushButton* lightGrayButton;
    QPushButton* redButton;
    QPushButton* yellowButton;
    QPushButton* greenButton;
    QPushButton* blueButton;
    QPushButton* magentaButton;
    QPushButton* cyanButton;

    void connectToSlots();

    widgetType type;

public slots:
    void slotOnXiButton();
    void slotOnZhongButton();
    void slotOnCuButton();

    //16个颜色按钮对应槽函数
    void slotOnBlackButton();
    void slotOnDarkGrayButton();
    void slotOnDarkRedButton();
    void slotOnOrangeButton();
    void slotOnDarkGreenButton();
    void slotOnDarkBlueButton();
    void slotOnDarkMagentaButton();
    void slotOnDarkCyanButton();

    void slotOnWhiteButton();
    void slotOnLightGrayButton();
    void slotOnRedButton();
    void slotOnYellowButton();
    void slotOnGreenButton();
    void slotOnBlueButton();
    void slotOnMagentaButton();
    void slotOnCyanButton();

};

#endif // COLORWIDGET_H
