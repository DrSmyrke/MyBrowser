#-------------------------------------------------
#
# Project created by QtCreator 2017-03-09T16:50:48
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = src
TEMPLATE = app
RC_FILE = index.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    myfunctions.cpp

HEADERS  += mainwindow.h \
    myfunctions.h

RESOURCES += \
    resources.qrc
