#if  !defined(_0_LUAUTILTIY_HPP)
#define _0_LUAUTILTIY_HPP

#include "QtUtility.hpp"

class QTUTILITYSHARED_EXPORT LuaUtility {
public:
    /*init utility lib*/
    static int openLib(lua_State * L);
    /*load utility module*/
    static void loadModule(lua_State * L);
    /*input a table out put a table*/
    static int copyTable( lua_State * L );
    /*input table name and table*/
    static int showTable(lua_State * L );
    /*input table name and table*/
    static int tableToString(lua_State * L );
protected:
    /*input table name and table*/
    static void printTable(lua_State * L ,std::function<void(const std::string &)> print_) ;
};

#endif // LUAUTILTIY_HPP




