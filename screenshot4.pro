QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arrowcommand.cpp \
    colorwidget.cpp \
    command.cpp \
    commandmanager.cpp \
    controlwidget.cpp \
    ellipsecommand.cpp \
    infolabel.cpp \
    main.cpp \
    myarrowitem.cpp \
    myellipseitem.cpp \
    mynumberitem.cpp \
    mynumbertextitem.cpp \
    mypenitem.cpp \
    myrectitem.cpp \
    mytextitem.cpp \
    numbercommand.cpp \
    order.cpp \
    pencommand.cpp \
    rectcommand.cpp \
    redomanager.cpp \
    screenshotview.cpp \
    selectcommand.cpp \
    selectitem.cpp \
    settings.cpp \
    textcommand.cpp \
    textsettings.cpp \
    textwidget.cpp \
    undomanager.cpp

HEADERS += \
    arrowcommand.h \
    colorwidget.h \
    command.h \
    commandmanager.h \
    controlwidget.h \
    ellipsecommand.h \
    infolabel.h \
    myarrowitem.h \
    myellipseitem.h \
    mynumberitem.h \
    mynumbertextitem.h \
    mypenitem.h \
    myrectitem.h \
    mytextitem.h \
    numbercommand.h \
    order.h \
    pencommand.h \
    rectcommand.h \
    redomanager.h \
    screenshotview.h \
    selectcommand.h \
    selectitem.h \
    settings.h \
    textcommand.h \
    textsettings.h \
    textwidget.h \
    undomanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
