#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T16:00:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DesktopCropper2
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    croppingwidget.cpp \
    screen.cpp \
    screenundowrapper.cpp

HEADERS  += mainwindow.h \
    croppingwidget.h \
    screen.h \
    monitor.h \
    screenundowrapper.h

FORMS    += mainwindow.ui
