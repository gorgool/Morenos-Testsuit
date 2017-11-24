TEMPLATE = app
TARGET = morenos_imitator
CONFIG += c++14

INCLUDEPATH += ../Packet_Manager

QMAKE_LIBDIR = ../Packet_Manager
LIBS = -lPacket_Manager -lpcap

HEADERS += \
  SimulationEngine.h

SOURCES += \
  main.cpp \
  SimulationEngine.cpp
