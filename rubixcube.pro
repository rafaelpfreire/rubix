#-------------------------------------------------
#
# Project created by QtCreator 2016-07-07T17:43:26
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rubixcube
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    cubepiece.cpp \
    rubixcube.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    cubepiece.h \
    rubixcube.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc