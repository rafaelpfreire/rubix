#------------------------------------------------------------
#
# Project developed by Rafael P. Freire
# Additional implementations by Mauro M. Araújo (Nintersoft)
# Project created by QtCreator 2016-07-07T17:43:26
#
#------------------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rubixcube
TEMPLATE = app

win32 {
    VERSION = 0.9.0.2

    QMAKE_TARGET_COMPANY = Rafael P. Freire (Nintersoft)
    QMAKE_TARGET_PRODUCT = Rubix
    QMAKE_TARGET_DESCRIPTION = Rubix Cube Game 3D!
    QMAKE_TARGET_COPYRIGHT = Copyright (c) 2016 Rafael P. Freire | Nintersoft

    RC_ICONS = Images\Icon.ico
    RC_LANG = 0x0416

    LIBS += -lOpengl32
}
else {
    VERSION = 0.9.0
}

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
        frmload.cpp \
        frmsettings.cpp

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
        frmload.h \
        frmsettings.h

FORMS    += mainwindow.ui \
            frmabout.ui \
            frmload.ui \
            frmsettings.ui

TRANSLATIONS = Languages\Rubix_pt.ts \
               Languages\Rubix_en.ts

RESOURCES += \
            resource.qrc
