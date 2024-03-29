#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include "textsettings.h"

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>

enum textWidgetType{
    text,
    serial
};

class textWidget : public QWidget
{
    Q_OBJECT
public:
    explicit textWidget(QWidget *parent = nullptr);

    textSettings* settings;

    void setType(textWidgetType type);
    void setLocationBelow();
    void setLocationAbove();

    void show();

private:
    textWidgetType type;

    void setButtons();
    void addWidgetToLayout();
    void setInitialLabel();

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

    //布局
    QHBoxLayout* totalLayout;
    QHBoxLayout* colorLayout1;
    QHBoxLayout* colorLayout2;
    QVBoxLayout* vColorLayout;

    //显示颜色的标签
    QLabel* choice;

    QComboBox* sizeComboBox;

    void connectToSlots();

public slots:
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

    void onComboBoxTextChanged(const QString& text);
};

#endif // TEXTWIDGET_H
