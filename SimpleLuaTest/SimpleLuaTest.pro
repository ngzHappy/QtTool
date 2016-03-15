CONFIG += c++14

QT += core
QT -= gui

TARGET = SimpleLuaTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += $$PWD/../QtUtility/lua/lua.cpp

include( $$PWD/../QtUtility/QtUtility.pri )
DESTDIR = $$QtUtilityOutPath
CONFIG(debug,debug|release){
LIBS +=-L$$QtUtilityOutPath -lQtUtilityd
}else{
LIBS +=-L$$QtUtilityOutPath -lQtUtility
}
