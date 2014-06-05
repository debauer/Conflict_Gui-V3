#-------------------------------------------------
#
# Project created by QtCreator 2014-02-05T21:41:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = V1
TEMPLATE = app

CONFIG += c++11
QT += uitools

CONFIG += static

include(qextserialport/src/qextserialport.pri)


SOURCES += main.cpp\
        mainwindow.cpp \
    conflict_core.cpp \
    carriage.cpp \
    driver/uart.cpp \
    conflictwidget.cpp \
    data/dfm.cpp \
    data/kanal.cpp \
    data/data.cpp \
    data/lcd.cpp \
    data/led.cpp \
    data/rechner.cpp \
    data/alarm.cpp \
    config.cpp \
    qcustomplot/qcustomplot.cpp \
    data/temperatur.cpp

HEADERS  += fetchThread.h \
    mainwindow.h \
    conflict_core.h \
    carriage.h \
    driver/uart.h \
    driver/absInterface.h \
    structs.h \
    conflictwidget.h \
    data/dfm.h \
    data/kanal.h \
    data/data.h \
    data/lcd.h \
    data/led.h \
    data/rechner.h \
    data/alarm.h \
    config.h \
    qcustomplot/qcustomplot.h \
    data/temperatur.h


FORMS    += \
    mainwindow.ui

OTHER_FILES +=
