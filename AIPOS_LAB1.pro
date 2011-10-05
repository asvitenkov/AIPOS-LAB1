#-------------------------------------------------
#
# Project created by QtCreator 2011-09-11T13:07:24
#
#-------------------------------------------------

QT       += core gui

TARGET = AIPOS_LAB1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    getmac.cpp \
    networkresources.cpp \
    progressbarwork.cpp

HEADERS  += mainwindow.h \
    getmac.h \
    networkresources.h \
    progressbarwork.h

FORMS    += mainwindow.ui

LIBS += -lnetapi32
LIBS += -lmpr
