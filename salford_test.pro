#-------------------------------------------------
#
# Project created by QtCreator 2015-11-18T09:10:55
#
#-------------------------------------------------

QT       += sql core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = salford_test
TEMPLATE = app


INCLUDEPATH += ./src

# Output directories
OBJECTS_DIR    = obj
UI_HEADERS_DIR = obj
UI_SOURCES_DIR = obj
DESTDIR        = $${_PRO_FILE_PWD_}/output

# SOURCES
SOURCES += src/main.cpp\
           src/mainwindow.cpp
# HEADERS
HEADERS  += src/mainwindow.h \
            src/commonparameters.h \
            src/connection.h \


FORMS    += src/mainwindow.ui
