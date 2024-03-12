#include "screenshotview.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    screenshotView* w = screenshotView::getInstance();
    w->show();
    return a.exec();
}
