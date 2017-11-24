TEMPLATE = lib
QT += core gui widgets
CONFIG += c++14 staticlib

OBJECTS_DIR = ../../build
MOC_DIR = ../../build
DESTDIR = ../../libs

SOURCES += \
    Composer.cpp \
    MenuItem.cpp

HEADERS += \
    Composer.h \
    MenuItem.h
