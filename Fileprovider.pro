#-------------------------------------------------
#
# Project created by QtCreator 2015-05-02T21:36:42
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Fileprovider
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH+= ../mangaviewer/include

DESTDIR = $$[QT_INSTALL_PLUGINS]/generic

SOURCES += fileprovider.cpp

HEADERS += fileprovider.h
DISTFILES += Fileprovider.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
