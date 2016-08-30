#-------------------------------------------------
#
# Project created by QtCreator 2016-07-07T17:43:26
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rubixcube
TEMPLATE = app

win32:LIBS += -lOpengl32

SOURCES += main.cpp\
        mainwindow.cpp \
        openglwidget.cpp \
        cubepiece.cpp \
        rubixcube.cpp \
        stopwatch.cpp \
        light.cpp \
        material.cpp \
        graphicobject.cpp \
        camera.cpp \
        frmabout.cpp \
    frmload.cpp

HEADERS  += mainwindow.h \
        openglwidget.h \
        cubepiece.h \
        rubixcube.h \
        stopwatch.h \
        light.h \
        material.h \
        graphicobject.h \
        camera.h \
        frmabout.h \
    frmload.h

FORMS    += mainwindow.ui \
            frmabout.ui \
    frmload.ui

RESOURCES += \
            resource.qrc
