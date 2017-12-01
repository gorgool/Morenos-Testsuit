TEMPLATE = app
QT += core gui widgets
TARGET = morenos_gui
CONFIG += c++14

INCLUDEPATH += ComposerWidget/ Data_Models/ PlotPositionWidget/ \
               PortraitPlaneWidget/ TargetTableWidget/ ../Packet_Manager/ ../Config_Manager \
               ../Exceptions

QMAKE_LIBDIR = ../libs/
LIBS = -lComposer -lData_Models -lPlotPosition -lPortraitPlane \
       -lTargetTable -lPacket_Manager -lpcap -lConfig_Manager \
       -lboost_filesystem -lboost_system -lPacketMonitor

MOC_DIR = ../build
OBJECTS_DIR = ../build
DESTDIR = ../bin

HEADERS += \
  GUI_Elements.h \
  TargetsModelAdapter.h

SOURCES += \
  GUIApplication.cpp \
  TargetsModelAdapter.cpp
