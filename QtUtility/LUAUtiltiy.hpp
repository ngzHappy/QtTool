#if  !defined(_0_LUAUTILTIY_HPP)
#define _0_LUAUTILTIY_HPP

#include <QtCore/qvariant.h>
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

class QTUTILITYSHARED_EXPORT LuaValue : public QVariant {
    typedef QVariant P;
public:
    using QVariant::QVariant;
    LuaValue(){}
    LuaValue(const LuaValue &)=default;
    LuaValue(LuaValue &&)=default;
    LuaValue&operator=(const LuaValue &)=default;
    LuaValue&operator=(LuaValue &&)=default;
    explicit LuaValue(const QVariant & value_):QVariant(value_){}
    explicit LuaValue(QVariant && value_):QVariant(std::move(value_)){}
    ~LuaValue()=default;
};

Q_DECLARE_METATYPE( LuaValue )

#endif // LUAUTILTIY_HPP




