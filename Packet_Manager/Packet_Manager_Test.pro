TEMPLATE = app
CONFIG += c++14

LIBS = -lpcap

MOC_DIR = ../build
OBJECTS_DIR = ../build
DESTDIR = ../bin

HEADERS += \
    Tests.h \
    PacketManager.h \
    PacketHeaders.h \
    SearchResult_Msg.h

SOURCES += \
    PacketManager.cpp \
    SearchResult_Msg.cpp \
    CodeDecodeTest.cpp \
    PlotDescriptionTest.cpp \
    SearchResultTest.cpp \
    SerializeDeserializeTest.cpp \
    PacketManagerTests.cpp
