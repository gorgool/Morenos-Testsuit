TEMPLATE = lib
QT += core gui widgets
CONFIG += c++14 staticlib

INCLUDEPATH += ../Data_Models/

SOURCES += \
    DescriptionWidget.cpp \
    TargetsTable.cpp

HEADERS += \
    DescriptionWidget.h \
    TargetsTable.h
