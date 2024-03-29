#include "textwidget.h"
#include "screenshotview.h"
#include "mytextitem.h"

#include <QPainter>

textWidget::textWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    settings = new textSettings();
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

void textWidget::setType(textWidgetType type){
    this->type = type;
}

void textWidget::setLocationAbove(){
    QRect controlRect = screenshotView::getInstance()->getControl()->getLocationRect();
    setGeometry(QRect(controlRect.x(),controlRect.y()-54,422,54));
}

void textWidget::setLocationBelow(){
    QRect controlRect = screenshotView::getInstance()->getControl()->getLocationRect();
    setGeometry(QRect(controlRect.x(),controlRect.y()+44,422,54));
}

void textWidget::setButtons(){
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

void textWidget::addWidgetToLayout(){
    QLabel* A = new QLabel();
    QFont font("Arial", 18);
    font.setBold(true);
    A->setFont(font);
    A->setFixedSize(QSize(50,50));
    A->setAlignment(Qt::AlignCenter);
    A->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    A->setText("A");
    totalLayout->addWidget(A);

    sizeComboBox = new QComboBox();

    sizeComboBox->setStyleSheet("QComboBox {"
                            "    border: 1px solid gray;"
                            "    border-radius: 3px;"
                            "    padding: 1px 10px 1px 3px;"
                            "    color:#666666;"
                            "    font-size:32px;"
                            "}"
                            );

    sizeComboBox->setFixedSize(102,50);
    QStringList options = {"8", "9", "10","11","12","14","16","18","20","22"};
    sizeComboBox->addItems(options);
    totalLayout->addWidget(sizeComboBox);

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

void textWidget::connectToSlots(){
    connect(blackButton,&QPushButton::clicked,this,&textWidget::slotOnBlackButton);
    connect(darkGrayButton,&QPushButton::clicked,this,&textWidget::slotOnDarkGrayButton);
    connect(darkRedButton,&QPushButton::clicked,this,&textWidget::slotOnDarkRedButton);
    connect(orangeButton,&QPushButton::clicked,this,&textWidget::slotOnOrangeButton);
    connect(darkGreenButton,&QPushButton::clicked,this,&textWidget::slotOnDarkGreenButton);
    connect(darkBlueButton,&QPushButton::clicked,this,&textWidget::slotOnDarkBlueButton);
    connect(darkMagentaButton,&QPushButton::clicked,this,&textWidget::slotOnDarkMagentaButton);
    connect(darkCyanButton,&QPushButton::clicked,this,&textWidget::slotOnDarkCyanButton);

    connect(whiteButton,&QPushButton::clicked,this,&textWidget::slotOnWhiteButton);
    connect(lightGrayButton,&QPushButton::clicked,this,&textWidget::slotOnLightGrayButton);
    connect(redButton,&QPushButton::clicked,this,&textWidget::slotOnRedButton);
    connect(yellowButton,&QPushButton::clicked,this,&textWidget::slotOnYellowButton);
    connect(greenButton,&QPushButton::clicked,this,&textWidget::slotOnGreenButton);
    connect(blueButton,&QPushButton::clicked,this,&textWidget::slotOnBlueButton);
    connect(magentaButton,&QPushButton::clicked,this,&textWidget::slotOnMagentaButton);
    connect(cyanButton,&QPushButton::clicked,this,&textWidget::slotOnCyanButton);

    connect(sizeComboBox,&QComboBox::currentTextChanged,this,&textWidget::onComboBoxTextChanged);
}

void textWidget::slotOnBlackButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::black);
        }
        settings->setTextColor(Qt::black);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void textWidget::slotOnDarkGrayButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::darkGray);
        }
        settings->setTextColor(Qt::darkGray);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnDarkRedButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch(type){
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::darkRed);
        }
        settings->setTextColor(Qt::darkRed);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnOrangeButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(QColor(255,165,0));
        }
        settings->setTextColor(QColor(255,165,0));
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnDarkGreenButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch(type){
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::darkGreen);
        }
        settings->setTextColor(Qt::darkGreen);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnDarkBlueButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::darkBlue);
        }
        settings->setTextColor(Qt::darkBlue);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnDarkMagentaButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        focusedTextItem->setDefaultTextColor(Qt::darkMagenta);
        settings->setTextColor(Qt::darkMagenta);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnDarkCyanButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::darkCyan);
        }
        settings->setTextColor(Qt::darkCyan);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnWhiteButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::white);
        }
        settings->setTextColor(Qt::white);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnLightGrayButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::lightGray);
        }
        settings->setTextColor(Qt::lightGray);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnRedButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::red);
        }
        settings->setTextColor(Qt::red);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnYellowButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::yellow);
        }
        settings->setTextColor(Qt::yellow);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnGreenButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        focusedTextItem->setDefaultTextColor(Qt::green);
        settings->setTextColor(Qt::green);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnBlueButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        focusedTextItem->setDefaultTextColor(Qt::blue);
        settings->setTextColor(Qt::blue);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnMagentaButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::magenta);
        }
        settings->setTextColor(Qt::magenta);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::slotOnCyanButton(){
    QString color;
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setDefaultTextColor(Qt::cyan);
        }
        settings->setTextColor(Qt::cyan);
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color:" + color);
}

void textWidget::setInitialLabel(){
    QString color;
    switch (type) {
    case textWidgetType::text:
        color = settings->getTextColor().name();
        break;
    default:
        color = "black";
        break;
    }
    choice->setStyleSheet("background-color: " + color);
}

void textWidget::show(){
    setInitialLabel();
    QWidget::show();
}

void textWidget::onComboBoxTextChanged(const QString& text){
    QGraphicsItem* focusedItem = screenshotView::getInstance()->getScene()->focusItem();
    myTextItem* focusedTextItem = qgraphicsitem_cast<myTextItem*>(focusedItem);
    QFont font;
    font.setPointSize(text.toInt());
    switch (type) {
    case textWidgetType::text:
        if(focusedTextItem){
            focusedTextItem->setFont(font);
        }
        settings->setTextSize(text.toInt());
        break;
    case textWidgetType::serial:
        break;
    }
}




















