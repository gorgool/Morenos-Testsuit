TEMPLATE = lib
QT += core gui widgets
CONFIG += c++14 staticlib

INCLUDEPATH += ../Data_Models/

MOC_DIR = ../../build
OBJECTS_DIR = ../../build
DESTDIR = ../../libs

SOURCES += \
    PortraitPlane.cpp \
    PortraitPlaneScene.cpp

HEADERS += \
    PortraitPlane.h \
    PortraitPlaneScene.h
