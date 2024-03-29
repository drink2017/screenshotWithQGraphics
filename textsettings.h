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

private:
    //文字item
    int textSize = 8;
    QColor textColor = Qt::black;
};

#endif // TEXTSETTINGS_H
