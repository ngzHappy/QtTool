
CONFIG += c++14

QT += core
QT += gui
QT += widgets

CONFIG(debug,debug|release){
TARGET = QtUtilityd
}else{
TARGET = QtUtility
}

TEMPLATE = lib

DEFINES += QTUTILITY_LIBRARY
DEFINES += QUAZIP_BUILD
DEFINES += LUA_BUILD_AS_DLL
win32{
}else{
DEFINES += LUA_USE_LINUX
}

SOURCES += $$PWD/source/QtUtility.cpp
SOURCES += $$PWD/source/QtLuaUtility.cpp

HEADERS += $$PWD/QtUtility.hpp
HEADERS += $$PWD/ReadOnly.hpp
HEADERS += $$PWD/qtutility_global.hpp

include($$PWD/quazip/quazip.pri)
include($$PWD/lua/lua.pri)
include($$PWD/QtUtility.pri)

DESTDIR = $$QtUtilityOutPath
