TEMPLATE = app
TARGET = morenos_imitator
CONFIG += c++14

INCLUDEPATH += ../Packet_Manager ../Config_Manager ../Exceptions

QMAKE_LIBDIR = ../libs
LIBS = -lPacket_Manager -lConfig_Manager -lpcap -lboost_system -lboost_filesystem

DESTDIR = ../bin
MOC_DIR = ../build
OBJECTS_DIR = ../build

HEADERS += \
  EventQueue.h \
  ModelEngine.h \
    SimulationState.h

SOURCES += \
    SimulationEngine.cpp \
    SimulationState.cpp \
    ModelEngine.cpp
