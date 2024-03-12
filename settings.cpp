#include "settings.h"

Settings::Settings()
{
    //设置默认设置
    rectColor = Qt::black;
    rectWidth = 2;

    ellipseColor = Qt::black;
    ellipseColor = 2;

    arrowColor = Qt::black;
    arrowWidth = 2;

    penColor = Qt::black;
    penWidth = 2;
}

//圆
void Settings::setRectColor(const QColor& color){
    rectColor = color;
}

QColor Settings::getRectColor() const{
    return rectColor;
}

void Settings::setRectWidth(int width){
    rectWidth = width;
}

int Settings::getRectWidth() const{
    return rectWidth;
}

//矩形
void Settings::setEllipseColor(const QColor &color){
    ellipseColor = color;
}

QColor Settings::getEllipseColor() const{
    return ellipseColor;
}

void Settings::setEllipseWidth(int width){
    ellipseWidth = width;
}

int Settings::getEllipseWidth() const{
    return ellipseWidth;
}

//箭头
void Settings::setArrowColor(const QColor &color){
    arrowColor = color;
}

QColor Settings::getArrowColor() const{
    return arrowColor;
}

void Settings::setArrowWidth(int width){
    arrowWidth = width;
}

int Settings::getArrowWidth() const{
    return arrowWidth;
}

//画笔
void Settings::setPenColor(const QColor &color){
    penColor = color;
}

QColor Settings::getPenColor() const{
    return penColor;
}

void Settings::setPenWidth(int width){
    penWidth = width;
}

int Settings::getPenWidth() const{
    return penWidth;
}






















