#-------------------------------------------------
#
# Project created by QtCreator 2017-03-09T16:50:48
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = src
TEMPLATE = app
RC_FILE = index.rc
QMAKE_CXXFLAGS += "-std=c++11"

TRANSLATIONS = lang/ru.ts
SOURCES += main.cpp\
        mainwindow.cpp \
    myfunctions.cpp \
    WebKit/webview.cpp \
    tabwidget.cpp \
    WebKit/webpage.cpp \
    javascriptobj.cpp \
    genpages.cpp

HEADERS  += mainwindow.h \
    myfunctions.h \
    WebKit/webview.h \
    tabwidget.h \
    WebKit/webpage.h \
    javascriptobj.h \
    genpages.h

RESOURCES += \
    resources.qrc
