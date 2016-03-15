#include "MainWiindow.hpp"
#include <QApplication>

#include <cassert>
#include <QtUtility.hpp>
#include <QDebug>
#include <iostream>

class TestShared5 : public std::shared_ptr<int> {
    typedef std::shared_ptr<int> P;
public:
    TestShared5() :P(new int(5)){}
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    {
       std::shared_ptr<lua_State> L_=LuaUtility::createLuaState( );
       lua_State * L=L_.get();
       luaL_dostring(L,"a={71,2,3}");
       luaL_dostring(L,"utility.showTable(\"a\",a)");
       LuaValue value(nullptr);
       lua_getglobal(L,"a");
       value.setLuaState(L_);
       value.setTable();
       lua_settop(L,0);
       lua_gettop(L);
       value.pushValue();

       if( lua_istable(L,-1) ) {
           lua_rawgeti(L,-1,1);
           qDebug()<<lua_tointeger(L,-1);
       }
       qDebug()<<"lua top:"<< lua_gettop(L);
       
       lua_pushinteger(L,3);
       lua_pushinteger(L,2);
       lua_pushinteger(L,1);
       lua_rotate(L,-3,1);
       //lua_rotate(L,-2,0);

       qDebug()
           <<lua_tointeger(L,-1)
           <<lua_tointeger(L,-2)
           <<lua_tointeger(L,-3);

       luaL_dostring(L,R"(return function() 
print("function call " .. tostring(199) ) 
end
)"
);
       value.setFunction();
       value();

       luaL_dostring(L,R"(return function(x,y,z) 
print("function call " .. tostring(x) .. " " .. tostring(y) .. " " .. tostring(z) ) 
end)");
       value.setFunction();
       lua_pushinteger(L,667);
       lua_pushinteger(L,668);
       lua_pushinteger(L,669);

       value(3);

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
        QSharedPointer<const int> value1(new int(1));
        int value2=2;
        TestShared5 value5;
        qDebug()<< readOnly(value2);
        qDebug()<< *readOnly(value1);
        qDebug()<< *readOnly(value0);
        qDebug()<< *readOnly(value5);
    }

    MainWiindow window;
    qDebug()<<"window is visible:"<<readOnly(window).isVisible();
    window.show();

    return app.exec();
}

/*
 * 简单的测试
*/
