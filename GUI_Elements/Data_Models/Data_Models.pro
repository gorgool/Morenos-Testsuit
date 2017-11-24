TEMPLATE = lib
QT += core gui widgets
CONFIG += c++14 staticlib

MOC_DIR = ../../build
OBJECTS_DIR = ../../build
DESTDIR = ../../libs

SOURCES += \
    TargetsModel.cpp

HEADERS += \
    TargetsModel.h \
    AntennaParamsModel.h \
    Target.h
