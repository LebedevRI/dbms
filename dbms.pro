#-------------------------------------------------
#
# Project created by QtCreator 2013-09-24T00:26:35
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dbms
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbconnparams.cpp \
    addnewexpert.cpp

HEADERS  += mainwindow.h \
    dbconnparams.h \
    addnewexpert.h

FORMS    += mainwindow.ui \
    dbconnparams.ui \
    addnewexpert.ui
