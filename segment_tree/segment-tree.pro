TEMPLATE = app 
TARGET = segment_tree

CONFIG += QtGui
QT += opengl

OBJECTS_DIR = bin

INCLUDEPATH += ../visualization/headers 

QMAKE_CXXFLAGS = -std=c++0x -Wall

macx {
    QMAKE_CXXFLAGS += -stdlib=libc++  
    QMAKE_LFLAGS += -lc++
}

CONFIG += precompile_header
PRECOMPILED_HEADER = stdafx.h

HEADERS += \
	common.h \
	primitives.h \
	segment_tree.h \
	stdafx.h \
	tree.h


SOURCES += \ 
	main.cpp

LIBS += -L../visualization -lvisualization
