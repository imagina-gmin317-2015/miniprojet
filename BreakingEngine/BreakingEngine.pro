TEMPLATE = app
QT += core gui network
TARGET = BreakinEngine
LIBS += -lnoise
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#Engine content
include(engine/BreakingEngine-engine.pri)
#Game content
include(game/BreakingEngine-game.pri)

SUBDIRS += engine game

RESOURCES += \
    engine.qrc

DISTFILES += \
    qtbreaking.vert \
    default.vs \
    shaders/water/water.vs \
    shaders/water/water.fs
