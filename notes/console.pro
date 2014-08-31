#-------------------------------------------------
#
#
#-------------------------------------------------

QT       += core sql xml

QT       -= gui
CONFIG   += debug
TARGET = notes
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app
QMAKE_CXXFLAGS += -pthread -Wunused-parameter -g
INCLUDEPATH += ../qslog
INCLUDEPATH += ..
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
include(../qslog/QsLog.pri)

SOURCES += main.cpp \
           notes.cpp \
           ..\place.cpp \
           task.cpp

HEADERS += task.h notes.h
