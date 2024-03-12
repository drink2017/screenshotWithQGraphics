#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>

class Settings
{
public:
    Settings();

    //设置和获取矩形颜色
    void setRectColor(const QColor& color);
    QColor getRectColor() const;
    //设置和获取矩形宽度
    void setRectWidth(int width);
    int getRectWidth() const;

    //设置和获取圆颜色
    void setEllipseColor(const QColor& color);
    QColor getEllipseColor() const;
    //设置和获取圆宽度
    void setEllipseWidth(int width);
    int getEllipseWidth() const;

    //设置和获取箭头颜色
    void setArrowColor(const QColor& color);
    QColor getArrowColor() const;
    //设置和获取箭头宽度
    void setArrowWidth(int width);
    int getArrowWidth() const;

    //设置和获取画笔颜色
    void setPenColor(const QColor& color);
    QColor getPenColor() const;
    //设置和获取画笔宽度
    void setPenWidth(int width);
    int getPenWidth() const;

private:
    //矩形颜色和宽度
    QColor rectColor;
    int rectWidth;

    //圆颜色和宽度
    QColor ellipseColor;
    int ellipseWidth;

    //箭头颜色和宽度
    QColor arrowColor;
    int arrowWidth;

    //画笔颜色和宽度
    QColor penColor;
    int penWidth;
};

#endif // SETTINGS_H
