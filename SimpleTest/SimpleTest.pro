CONFIG += c++14
CONFIG += console

QT += core gui
QT += widgets

TARGET = SimpleTest
TEMPLATE = app


SOURCES += main.cpp \
    TextEdit.cpp \
    TextLayout.cpp
SOURCES += MainWiindow.cpp

HEADERS  += $$PWD/MainWiindow.hpp \
    TextEdit.hpp \
    TextLayout.hpp

include( $$PWD/../QtUtility/QtUtility.pri )
DESTDIR = $$QtUtilityOutPath
CONFIG(debug,debug|release){
LIBS +=-L$$QtUtilityOutPath -lQtUtilityd
}else{
LIBS +=-L$$QtUtilityOutPath -lQtUtility
}

