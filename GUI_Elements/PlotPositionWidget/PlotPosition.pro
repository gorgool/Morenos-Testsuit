TEMPLATE = lib
QT += core gui widgets
CONFIG += c++14 staticlib

INCLUDEPATH += ../Data_Models/

SOURCES += \
    PlotPosition.cpp \
    PlotPositionScene.cpp

HEADERS += \
    PlotPosition.h \
    PlotPositionScene.h