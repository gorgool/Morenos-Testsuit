TEMPLATE = lib
QT += core gui widgets
CONFIG += c++14 staticlib

INCLUDEPATH += ../Data_Models/ ../../Packet_Generator

MOC_DIR = ../../build
OBJECTS_DIR = ../../build
DESTDIR = ../../libs

SOURCES += \
    PacketMonitor.cpp \
    MessagesModel.cpp \
    MessagesListModel.cpp

HEADERS += \
    PacketMonitor.h \
    MessagesModel.h \
    MessagesListModel.h
