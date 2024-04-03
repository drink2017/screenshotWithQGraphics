#ifndef TEXTSETTINGS_H
#define TEXTSETTINGS_H

#include <QColor>

class textSettings
{
public:
    textSettings();

    //设置和获取文字item尺寸
    void setTextSize(int textSize);
    int getTextSize();
    //设置和获取文字item颜色
    void setTextColor(QColor textColor);
    QColor getTextColor();

    //设置和获取序号item尺寸
    void setNumberSize(int numberSize);
    int getNumberSize();
    //设置和获取序号item颜色
    void setNumberColor(QColor numberColor);
    QColor getNumberColor();

private:
    //文字item
    int textSize = 8;
    QColor textColor = Qt::black;

    //序号item
    int numberSize = 8;
    QColor numberColor = Qt::black;
};

#endif // TEXTSETTINGS_H
