#-------------------------------------------------
#
# Project created by QtCreator 2017-03-15T23:30:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QWT500
TEMPLATE = app

#TEMPLATE = lib
#DEFINES += BUILD_DLL

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -L"$$_PRO_FILE_PWD_/yokogawa" -ltmctl64

SOURCES += main.cpp\
        qwt500widget.cpp \
    qwt500.cpp \
    qwt500item.cpp \
    mData/mdata.cpp \
    mData/mdatahandler.cpp \
    mData/mdatalogger.cpp

HEADERS  += qwt500widget.h \
    qwt500.h \
    qwt500item.h \
    yokogawa/tmctl.h \
    mData/mdata.h \
    mData/mdatahandler.h \
    mData/mdatalogger.h

FORMS    += qwt500widget.ui
