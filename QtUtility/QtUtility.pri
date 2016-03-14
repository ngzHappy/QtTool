#git submodule add https://github.com/ngzHappy/QtTool
INCLUDEPATH += $$PWD
#include($$PWD/../QtTool/QtUtility/QtUtility.pri)
#DESTDIR = $$QtUtilityOutPath
#CONFIG(debug,debug|release){
#LIBS +=-L$$QtUtilityOutPath -lQtUtilityd
#}else{
#LIBS +=-L$$QtUtilityOutPath -lQtUtility
#}

win32-msvc*{
##msvc
CONFIG(debug,debug|release){
QtUtilityOutPath = $$PWD/../binlib/msvc/bind
}else{
QtUtilityOutPath = $$PWD/../binlib/msvc/bin
}

}else{

win32{
##win32
CONFIG(debug,debug|release){
QtUtilityOutPath = $$PWD/../binlib/win32/bind
}else{
QtUtilityOutPath = $$PWD/../binlib/win32/bin
}

}else{
##linux macx
CONFIG(debug,debug|release){
QtUtilityOutPath = $$PWD/../binlib/common/bind
}else{
QtUtilityOutPath = $$PWD/../binlib/common/bin
}

}

}



