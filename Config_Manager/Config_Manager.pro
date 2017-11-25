TEMPLATE = lib
CONFIG += c++14 staticlib

INCLUDEPATH += ../Exceptions/

MOC_DIR = ../build
OBJECTS_DIR = ../build
DESTDIR = ../libs

HEADERS += \
    ConfigManager.hpp

SOURCES += \
    ConfigManager.cpp
