#-------------------------------------------------
#
# Project created by QtCreator 2016-03-24T09:00:21
#
#-------------------------------------------------

QT       += core gui
QT       +=serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imageGrapAssistant
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mylabel.cpp \
    myimage.cpp \
    aboutdialog.cpp \
    motordrive.cpp \
    picedit.cpp \
    mypen.cpp

HEADERS  += mainwindow.h \
    mylabel.h \
    myimage.h \
    aboutdialog.h \
    motordrive.h \
    picedit.h \
    mypen.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    picedit.ui

RESOURCES += \
    images.qrc

DISTFILES += \
    x-y.png
