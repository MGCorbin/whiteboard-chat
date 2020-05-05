#-------------------------------------------------
#
# Project created by QtCreator 2015-02-04T13:43:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab20
TEMPLATE = app


SOURCES += main.cpp \
    drawarea.cpp \
    receivewindow.cpp \
    sendwindow.cpp \
    serial.cpp \
    test.cpp

HEADERS  += \
    drawarea.h \
    receivewindow.h \
    safequeue.h \
    sendwindow.h \
    serial.h \
    test.h

FORMS    += \
    receivewindow.ui \
    sendwindow.ui

LIBS += -L/usr/local/lib -lpthread
