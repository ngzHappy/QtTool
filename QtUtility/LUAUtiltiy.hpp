#if  !defined(_0_LUAUTILTIY_HPP)
#define _0_LUAUTILTIY_HPP

#include "QtUtility.hpp"

class QTUTILITYSHARED_EXPORT LuaUtility {
public:
    /*create lua state*/
    static std::shared_ptr<lua_State> createLuaState();
    static std::shared_ptr<lua_State> createLuaState(lua_State*);
    /*init utility lib*/
    static int openLib(lua_State * L);
    /*load utility module*/
    static void loadModule(lua_State * L);
    /*input a table out put a table*/
    static int copyTable(lua_State * L);
    /*input table name and table*/
    static int showTable(lua_State * L);
    /*input table name and table*/
    static int tableToString(lua_State * L);
protected:
    /*input table name and table*/
    static void printTable(lua_State * L,std::function<void(const std::string &)> print_);
};

/*这个类被设计用于表达 Function 和 Table*/
class QTUTILITYSHARED_EXPORT LuaValue {
public:
    typedef int Type;
    enum : int {
        Type_NIL,
        Type_Function,
        Type_Table,
        Type_Count
    };
private:
    class TypePointer_ {
    public:
        char pointersSet[4]; char tablePath[4];
        TypePointer_() {
            pointersSet[0]='p'; pointersSet[1]='s'; pointersSet[2]='e'; pointersSet[3]='t';
            tablePath[0]='t'; tablePath[1]='p'; tablePath[2]='t'; tablePath[3]='h';
        }
    };
    static TypePointer_ type_pointer_[Type_Count];
    class Data_ {
    public:
        std::weak_ptr<lua_State> luaState_;
        int type_=Type_NIL;
        int pointer_=0;
    };
    std::shared_ptr<Data_> data_;
public:

    LuaValue();
    LuaValue(decltype(nullptr)) {/**/ }
    virtual ~LuaValue();

    QVariant getValue() const;
    void setTable();
    void setFunction();

    void pushValue() const;

    std::shared_ptr<lua_State> getLuaState() const {
        if (data_) { return data_->luaState_.lock(); }
        return nullptr;
    }

    void setLuaState(std::shared_ptr<lua_State> &&value_) {
        if (bool(data_)==false) { data_=std::shared_ptr<Data_>(new Data_); }
        data_->luaState_=std::move(value_);
    }

    void setLuaState(const std::shared_ptr<lua_State> &value_) {
        if (bool(data_)==false) { data_=std::shared_ptr<Data_>(new Data_); }
        data_->luaState_=value_;
    }

    Type getType()const noexcept { return data_->type_; }

    explicit operator bool()const { return bool(data_); }

    void operator()(int argc=0);

protected:
    static constexpr void * getTablePathPointer(Type v) noexcept { return (&(type_pointer_[v].tablePath)); }
    static constexpr void * getTypeSetPointer(Type v) noexcept { return (&(type_pointer_[v].pointersSet)); }
    void clear();
    static void* getTypeSet(lua_State *,Type v);

private:
    template< Type >void _setValue(lua_State *);
};

Q_DECLARE_METATYPE(LuaValue)

#endif // LUAUTILTIY_HPP




