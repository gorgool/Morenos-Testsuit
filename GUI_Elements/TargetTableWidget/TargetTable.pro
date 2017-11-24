TEMPLATE = lib
QT += core gui widgets
CONFIG += c++14 staticlib

INCLUDEPATH += ../Data_Models/

MOC_DIR = ../../build
OBJECTS_DIR = ../../build
DESTDIR = ../../libs

SOURCES += \
    DescriptionWidget.cpp \
    TargetsTable.cpp

HEADERS += \
    DescriptionWidget.h \
    TargetsTable.h
