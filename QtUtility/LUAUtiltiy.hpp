#if  !defined(_0_LUAUTILTIY_HPP)
#define _0_LUAUTILTIY_HPP

#include "QtUtility.hpp"

class QTUTILITYSHARED_EXPORT LuaUtility {
public:
    static int copyTable( lua_State * L );
    static void printTable(
            lua_State * L ,
            std::function<void(const std::string &)> print_
            ) ;
};

#endif // LUAUTILTIY_HPP




