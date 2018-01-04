#-------------------------------------------------
#
# Project created by QtCreator 2017-03-09T16:50:48
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myBrowser
TEMPLATE = app
win32|win64{
    RC_FILE=  index.rc
    OTHER_FILES+= index.rc
    DISTFILES += index.rc
}
QMAKE_CXXFLAGS += "-std=c++11"

TRANSLATIONS = lang/ru_RU.ts
SOURCES += main.cpp\
        mainwindow.cpp \
    global.cpp \
    WebKit/webview.cpp \
    tabwidget.cpp \
    WebKit/webpage.cpp \
    javascriptobj.cpp \
    genpages.cpp

HEADERS  += mainwindow.h \
    global.h \
    WebKit/webview.h \
    tabwidget.h \
    WebKit/webpage.h \
    javascriptobj.h \
    genpages.h

RESOURCES += \
    resources.qrc
