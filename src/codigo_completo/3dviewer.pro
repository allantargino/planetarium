#-------------------------------------------------
#
# Project created by QtCreator 2016-06-14T22:03:23
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3dviewer
TEMPLATE = app

CONFIG += c++14

SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    light.cpp \
    material.cpp \
    camera.cpp \
    trackball.cpp \
    model.cpp \
    modelfactory.cpp \
    astronomicalobject.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    light.h \
    material.h \
    camera.h \
    trackball.h \
    model.h \
    modelfactory.h \
    astronomicalobject.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc \
    resources.qrc
