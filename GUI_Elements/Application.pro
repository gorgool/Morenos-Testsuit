TEMPLATE = app
QT += core gui widgets
TARGET = morenos_gui
CONFIG += c++14

INCLUDEPATH += ComposerWidget/ Data_Models/ PlotPositionWidget/ PortraitPlaneWidget/ TargetTableWidget/ ../Packet_Manager/

QMAKE_LIBDIR = ComposerWidget Data_Models PlotPositionWidget PortraitPlaneWidget TargetTableWidget ../Packet_Manager
LIBS = -lComposer -lData_Models -lPlotPosition -lPortraitPlane -lTargetTable -lPacket_Manager -lpcap

HEADERS += \
  GUI_Elements.h \
  TargetsModelAdapter.h

SOURCES += \
  main.cpp \
  TargetsModelAdapter.cpp
