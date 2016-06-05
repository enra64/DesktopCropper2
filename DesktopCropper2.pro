#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T16:00:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DesktopCropper2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    croppingwidget.cpp \
    screen.cpp

HEADERS  += mainwindow.h \
    croppingwidget.h \
    screen.h \
    monitor.h

FORMS    += mainwindow.ui
