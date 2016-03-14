#include "MainWiindow.hpp"
#include <QApplication>

#include <QtUtility.hpp>
#include <QDebug>
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

    {
        std::shared_ptr<int> value0(new int(0));
        QSharedPointer<int> value1(new int(1));
        int value2=2;
        qDebug()<< readOnly(value2);
        qDebug()<< *readOnly(value1);
        qDebug()<< *readOnly(value0);
    }

    MainWiindow window;
    qDebug()<<"window is visible:"<<readOnly(window).isVisible();
    window.show();

    return app.exec();
}

/*
 * 简单的测试
*/
