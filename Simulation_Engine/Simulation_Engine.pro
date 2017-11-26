TEMPLATE = app
TARGET = morenos_imitator
CONFIG += c++14

INCLUDEPATH += ../Packet_Manager

QMAKE_LIBDIR = ../libs
LIBS = -lPacket_Manager -lpcap

DESTDIR = ../bin
MOC_DIR = ../build
OBJECTS_DIR = ../build

HEADERS += \
  EventQueue.h \
  ModelEngine.h \
    SimulationState.h

SOURCES += \
  main.cpp \
    SimulationState.cpp \
    ModelEngine.cpp
