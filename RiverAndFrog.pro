#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T14:13:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RiverAndFrog
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    view.cpp \
    model.cpp \
    controller.cpp

HEADERS  += mainwindow.h \
    view.h \
    model.h \
    controller.h

FORMS    += mainwindow.ui \
    view.ui
