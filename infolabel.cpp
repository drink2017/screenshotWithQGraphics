#include "infolabel.h"
#include "screenshotview.h"

#include <QPalette>

infoLabel::infoLabel()
{
    QPalette palette;
    palette.setColor(QPalette::Background,Qt::black);
    palette.setColor(QPalette::WindowText,Qt::white);
    setPalette(palette);

    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    QPoint selectStart = screenshotView::getInstance()->getSelectStart();
    QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();
    setLocation(selectStart,selectEnd);
    setAlignment(Qt::AlignCenter);
    setText(getInfo(selectStart,selectEnd));
}

void infoLabel::setLocation(QPoint selectStart, QPoint selectEnd){
    QRect selectRect = QRect(selectStart,selectEnd);
    if(selectRect.top() < 21){
        this->setGeometry(selectRect.topLeft().x(),selectRect.topLeft().y(),100,21);
    }else{
        this->setGeometry(selectRect.topLeft().x(),selectRect.topLeft().y()-21,100,21);
    }
}

QString infoLabel::getInfo(QPoint selectStart, QPoint selectEnd){
    int width = abs(selectEnd.x()-selectStart.x());
    int height = abs(selectEnd.y()-selectStart.y());
    return QString::number(width) + "*" + QString::number(height);
}

void infoLabel::updateInfoLabel(){
    QPoint selectStart = screenshotView::getInstance()->getSelectStart();
    QPoint selectEnd = screenshotView::getInstance()->getSelectEnd();
    setLocation(selectStart,selectEnd);
    setText(getInfo(selectStart,selectEnd));
}
