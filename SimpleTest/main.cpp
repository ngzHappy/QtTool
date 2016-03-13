#include "MainWiindow.hpp"
#include <QApplication>

#include <QtUtility.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    {
       lua_State * L = luaL_newstate();
       luaL_openlibs( L );
       luaL_dostring(L,"print(\"Hellow Word!\")");
       lua_close( L );
    }

    {
        QtUtility::addDestructFunction([](){std::cout<<"destruct 0"<<std::endl;});
        QtUtility::addDestructFunction([](){std::cout<<"destruct 1"<<std::endl;});
    }

    {
        QuaZip zip;
        zip.isOpen();
    }

    MainWiindow window;
    window.show();

    return app.exec();
}

/*
 * 简单的测试
*/
