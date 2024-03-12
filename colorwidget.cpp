#include "colorwidget.h"
#include "screenshotview.h"

#include <QDebug>

colorWidget::colorWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    settings = new Settings();
    setButtons();
    totalLayout = new QHBoxLayout(this);
    this->setLayout(totalLayout);
    colorLayout1 = new QHBoxLayout;
    colorLayout2 = new QHBoxLayout;
    vColorLayout = new QVBoxLayout;
    totalLayout->setSpacing(2);
    totalLayout->setContentsMargins(2,2,2,2);

    addWidgetToLayout();

    connectToSlots();
}

void colorWidget::setButtons(){
    xiButton = new QPushButton(this);
    xiButton->setFixedSize(QSize(50,50));
    xiButton->setText("细");
    xiButton->setCheckable(true);

    zhongButton = new QPushButton(this);
    zhongButton->setFixedSize(QSize(50,50));
    zhongButton->setText("中");
    zhongButton->setCheckable(true);

    cuButton = new QPushButton(this);
    cuButton->setFixedSize(QSize(50,50));
    cuButton->setText("粗");
    cuButton->setCheckable(true);

    blackButton = new QPushButton(this);
    blackButton->setFixedSize(QSize(24,24));
    blackButton->setStyleSheet("background-color: black");

    darkGrayButton = new QPushButton(this);
    darkGrayButton->setFixedSize(QSize(24,24));
    darkGrayButton->setStyleSheet("background-color: darkGray");

    darkRedButton = new QPushButton(this);
    darkRedButton->setFixedSize(QSize(24,24));
    darkRedButton->setStyleSheet("background-color: darkRed");

    orangeButton = new QPushButton(this);
    orangeButton->setFixedSize(QSize(24,24));
    orangeButton->setStyleSheet("background-color: orange");

    darkGreenButton = new QPushButton(this);
    darkGreenButton->setFixedSize(QSize(24,24));
    darkGreenButton->setStyleSheet("background-color: darkGreen");

    darkBlueButton = new QPushButton(this);
    darkBlueButton->setFixedSize(QSize(24,24));
    darkBlueButton->setStyleSheet("background-color: darkBlue");

    darkMagentaButton = new QPushButton(this);
    darkMagentaButton->setFixedSize(QSize(24,24));
    darkMagentaButton->setStyleSheet("background-color: darkMagenta");

    darkCyanButton = new QPushButton(this);
    darkCyanButton->setFixedSize(QSize(24,24));
    darkCyanButton->setStyleSheet("background-color: darkCyan");

    whiteButton = new QPushButton(this);
    whiteButton->setFixedSize(QSize(24,24));
    whiteButton->setStyleSheet("background-color: white");

    lightGrayButton = new QPushButton(this);
    lightGrayButton->setFixedSize(QSize(24,24));
    lightGrayButton->setStyleSheet("background-color: lightGray");

    redButton = new QPushButton(this);
    redButton->setFixedSize(QSize(24,24));
    redButton->setStyleSheet("background-color:red");

    yellowButton = new QPushButton(this);
    yellowButton->setFixedSize(QSize(24,24));
    yellowButton->setStyleSheet("background-color: yellow");

    greenButton = new QPushButton(this);
    greenButton->setFixedSize(QSize(24,24));
    greenButton->setStyleSheet("background-color: green");

    blueButton = new QPushButton(this);
    blueButton->setFixedSize(QSize(24,24));
    blueButton->setStyleSheet("background-color: blue");

    magentaButton = new QPushButton(this);
    magentaButton->setFixedSize(QSize(24,24));
    magentaButton->setStyleSheet("background-color: magenta");

    cyanButton = new QPushButton(this);
    cyanButton->setFixedSize(QSize(24,24));
    cyanButton->setStyleSheet("background-color: cyan");
}

void colorWidget::setInitialButton(){
    switch(type){
    case widgetType::rect:
        switch (settings->getRectWidth()) {
        case 2:
            xiButton->setChecked(true);
            break;
        case 4:
            zhongButton->setChecked(true);
            break;
        case 6:
            cuButton->setChecked(true);
            break;
        }
    default:
        break;
    }
}

void colorWidget::setInitialLabel(){
    QString color;
    switch (type) {
    case widgetType::rect:
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::setLocationAbove(){
    QRect controlRect = screenshotView::getInstance()->getControl()->getLocationRect();
    setGeometry(QRect(controlRect.x(),controlRect.y()-54,422,54));
}

void colorWidget::setLocationBelow(){
    QRect controlRect = screenshotView::getInstance()->getControl()->getLocationRect();
    setGeometry(QRect(controlRect.x(),controlRect.y()+44,422,54));
}

void colorWidget::addWidgetToLayout(){
    totalLayout->addWidget(xiButton);
    totalLayout->addWidget(zhongButton);
    totalLayout->addWidget(cuButton);

    QFrame* verticalLine = new QFrame;
    verticalLine->setFrameShape(QFrame::VLine);
    verticalLine->setFrameShadow(QFrame::Sunken);
    verticalLine->setFixedWidth(2);
    totalLayout->addWidget(verticalLine);

    choice = new QLabel;
    choice->setFixedSize(QSize(50,50));
    totalLayout->addWidget(choice);

    //将第一排按钮加到布局
    colorLayout1->addWidget(blackButton);
    colorLayout1->addWidget(darkGrayButton);
    colorLayout1->addWidget(darkRedButton);
    colorLayout1->addWidget(orangeButton);
    colorLayout1->addWidget(darkGreenButton);
    colorLayout1->addWidget(darkBlueButton);
    colorLayout1->addWidget(darkMagentaButton);
    colorLayout1->addWidget(darkCyanButton);

    //将第二排按钮加到布局
    colorLayout2->addWidget(whiteButton);
    colorLayout2->addWidget(lightGrayButton);
    colorLayout2->addWidget(redButton);
    colorLayout2->addWidget(yellowButton);
    colorLayout2->addWidget(greenButton);
    colorLayout2->addWidget(blueButton);
    colorLayout2->addWidget(magentaButton);
    colorLayout2->addWidget(cyanButton);

    vColorLayout->addLayout(colorLayout1);
    vColorLayout->addLayout(colorLayout2);
    totalLayout->addLayout(vColorLayout);
}

void colorWidget::setType(widgetType type){
    this->type = type;
}

void colorWidget::connectToSlots(){
    connect(xiButton,&QPushButton::clicked,this,&colorWidget::slotOnXiButton);
    connect(zhongButton,&QPushButton::clicked,this,&colorWidget::slotOnZhongButton);
    connect(cuButton,&QPushButton::clicked,this,&colorWidget::slotOnCuButton);

    connect(blackButton,&QPushButton::clicked,this,&colorWidget::slotOnBlackButton);
    connect(darkGrayButton,&QPushButton::clicked,this,&colorWidget::slotOnDarkGrayButton);
    connect(darkRedButton,&QPushButton::clicked,this,&colorWidget::slotOnDarkRedButton);
    connect(orangeButton,&QPushButton::clicked,this,&colorWidget::slotOnOrangeButton);
    connect(darkGreenButton,&QPushButton::clicked,this,&colorWidget::slotOnDarkGreenButton);
    connect(darkBlueButton,&QPushButton::clicked,this,&colorWidget::slotOnDarkBlueButton);
    connect(darkMagentaButton,&QPushButton::clicked,this,&colorWidget::slotOnDarkMagentaButton);
    connect(darkCyanButton,&QPushButton::clicked,this,&colorWidget::slotOnDarkCyanButton);

    connect(whiteButton,&QPushButton::clicked,this,&colorWidget::slotOnWhiteButton);
    connect(lightGrayButton,&QPushButton::clicked,this,&colorWidget::slotOnLightGrayButton);
    connect(redButton,&QPushButton::clicked,this,&colorWidget::slotOnRedButton);
    connect(yellowButton,&QPushButton::clicked,this,&colorWidget::slotOnYellowButton);
    connect(greenButton,&QPushButton::clicked,this,&colorWidget::slotOnGreenButton);
    connect(blueButton,&QPushButton::clicked,this,&colorWidget::slotOnBlueButton);
    connect(magentaButton,&QPushButton::clicked,this,&colorWidget::slotOnMagentaButton);
    connect(cyanButton,&QPushButton::clicked,this,&colorWidget::slotOnCyanButton);
}

void colorWidget::slotOnXiButton(){
    if(xiButton->isChecked()){
        zhongButton->setChecked(false);
        cuButton->setChecked(false);
        switch (type) {
        case widgetType::rect:
            settings->setRectWidth(2);
            break;
        default:
            break;
        }
    }else{
        xiButton->setChecked(true);
    }
}

void colorWidget::slotOnZhongButton(){
    if(zhongButton->isChecked()){
        xiButton->setChecked(false);
        cuButton->setChecked(false);
        switch (type) {
        case widgetType::rect:
            settings->setRectWidth(4);
            break;
        default:
            break;
        }
    }else{
        zhongButton->setChecked(true);
    }
}

void colorWidget::slotOnCuButton(){
    if(cuButton->isChecked()){
        xiButton->setChecked(false);
        zhongButton->setChecked(false);
        switch (type) {
        case widgetType::rect:
            settings->setRectWidth(6);
            break;
        default:
            break;
        }
    }else{
        cuButton->setChecked(true);
    }
}

void colorWidget::slotOnBlackButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::black);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnDarkGrayButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::darkGray);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnDarkRedButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::darkRed);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnOrangeButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(QColor(255,165,0));
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnDarkGreenButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::darkGreen);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnDarkBlueButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::darkBlue);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnDarkMagentaButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::darkMagenta);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnDarkCyanButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::darkCyan);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnWhiteButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::white);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnLightGrayButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::lightGray);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnRedButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::red);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnYellowButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::yellow);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnGreenButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::green);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnBlueButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::blue);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnMagentaButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::magenta);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::slotOnCyanButton(){
    QString color;
    switch (type) {
    case widgetType::rect:
        settings->setRectColor(Qt::cyan);
        color = settings->getRectColor().name();
        break;
    case widgetType::ellipse:
        color = settings->getEllipseColor().name();
        break;
    case widgetType::arrow:
        color = settings->getArrowColor().name();
        break;
    case widgetType::pen:
        color = settings->getPenColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void colorWidget::show(){
    setInitialButton();
    setInitialLabel();
    QWidget::show();
}

















