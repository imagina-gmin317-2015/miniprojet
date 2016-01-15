HEADERS+= \
    $$PWD/scene1.h \
    $$PWD/terrain.h
SOURCES+= $$PWD/main.cpp \
    $$PWD/scene1.cpp \
    $$PWD/terrain.cpp
#include(../engine/BreakingEngine-engine.pri)

RESOURCES += \
    $$PWD/game.qrc
