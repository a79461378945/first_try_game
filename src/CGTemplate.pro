QT += core gui opengl

QT   += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += \
	Glu32.lib \
	OpenGL32.lib

SOURCES += \
    1_myglwidget.cpp \
    2_argument.cpp \
    2_game.cpp \
    2_level_data.cpp \
    2_read_bmp.cpp \
    3_background_point.cpp \
    3_blackhole.cpp \
    3_bullet.cpp \
    3_fan.cpp \
    3_my_plane.cpp \
    4_entity_and_point.cpp \
    4_spark.cpp \
    main.cpp \
    myui.cpp

HEADERS += \
    argument.h \
    background_point.h \
    blackhole.h \
    bullet.h \
    entity_and_point.h \
    fan.h \
    game.h \
    level_data.h \
    my_plane.h \
    myglwidget.h \
    myui.h \
    read_bmp.h \
    spark.h

FORMS += \
    myui.ui
