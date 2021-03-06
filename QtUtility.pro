TEMPLATE = subdirs

#.subdir #Use the specified subdirectory instead of SUBDIRS value.
#.file #Specify the subproject pro file explicitly. Cannot be used in conjunction with .subdir modifier.
#.depends #This subproject depends on specified subproject.
#.makefile #The makefile of subproject. Available only on platforms that use makefiles.
#.target #Base string used for makefile targets related to this subproject. Available only on platforms that use makefiles.

QtUtility.file = $$PWD/QtUtility/QtUtility.pro
SimpleTest.file = $$PWD/SimpleTest/SimpleTest.pro
SimpleTest.depends += QtUtility
SimpleLuaTest.file = $$PWD/SimpleLuaTest/SimpleLuaTest.pro
SimpleLuaTest.depends += QtUtility

SUBDIRS += QtUtility
SUBDIRS += SimpleLuaTest
SUBDIRS += SimpleTest













