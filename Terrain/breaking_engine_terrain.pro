include(breaking_engine_terrain.pri)

SOURCES += \
    main.cpp \
    terrain.cpp \
    quadtree.cpp \
    voxel.cpp \
    gui.cpp \
    breakingnoise.cpp \
    noiseutils.cpp \
    gradientcolorwidget.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
INSTALLS += target

HEADERS += \
    terrain.h \
    quadtree.h \
    voxel.h \
    commonstruct.h \
    gui.h \
    breakingnoise.h \
    noiseutils.h \
    gradientcolorwidget.h

QT += opengl widgets

QMAKE_CXXFLAGS += -std=c++11
LIBS += -std=c++11 -lnoise

RESOURCES +=
