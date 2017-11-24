TEMPLATE = lib
CONFIG += c++14 staticlib

LIBS = -lpcap

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
    SerializeDeserializeTest.cpp
