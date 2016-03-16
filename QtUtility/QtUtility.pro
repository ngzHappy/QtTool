
CONFIG += c++14

QT += core
QT += gui
QT += widgets

QT += core_private
QT += gui_private
QT += widgets_private

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
SOURCES += $$PWD/source/QtTextDocumentLayout.cpp

HEADERS += $$PWD/QtUtility.hpp
HEADERS += $$PWD/LUAUtiltiy.hpp
HEADERS += $$PWD/ReadOnly.hpp
HEADERS += $$PWD/qtutility_global.hpp
HEADERS += $$PWD/QtTextDocumentLayout.hpp

include($$PWD/quazip/quazip.pri)
include($$PWD/lua/lua.pri)
include($$PWD/QtUtility.pri)

DESTDIR = $$QtUtilityOutPath
