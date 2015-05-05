#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T18:09:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:RC_ICONS += icon.ico
VERSION = 1.0.0.0
QMAKE_TARGET_COMPANY = Niun Apps
QMAKE_TARGET_PRODUCT = Mod Manager V
QMAKE_TARGET_DESCRIPTION = Modding your life!
QMAKE_TARGET_COPYRIGHT = Attribution-NonCommercial-NoDerivatives 4.0 International

TARGET = ModManagerV
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    manager.cpp

HEADERS  += mainwindow.h \
    manager.h

FORMS    += mainwindow.ui

TRANSLATIONS = modmanager_en.ts \
               modmanager_br.ts

RESOURCES += \
    res.qrc
