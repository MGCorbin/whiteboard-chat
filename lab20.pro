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
    receiver.cpp \
    receivewindow.cpp \
    sender.cpp \
    sendwindow.cpp \
    viewarea.cpp

HEADERS  += \
    drawarea.h \
    receiver.h \
    receivewindow.h \
    safequeue.h \
    sender.h \
    sendwindow.h \
    typedefs.h \
    viewarea.h

FORMS    += \
    receivewindow.ui \
    sendwindow.ui

LIBS += -L/usr/local/lib -lpthread
