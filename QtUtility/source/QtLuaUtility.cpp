#define LUA_LIB
/*this file is + lua library*/

#include "QtUtility.hpp"

#include <sstream>
#include <iostream>

#if !defined(FLOAT_PRECISION)
namespace {
enum { FLOAT_PRECISION_=16 };
}
#else
namespace {
enum { FLOAT_PRECISION_=FLOAT_PRECISION };
}
#endif

/*
 * static data 生成唯一地址
*/
LuaValue::TypePointer_ LuaValue::type_pointer_[LuaValue::Type_Count];

namespace {
namespace __cct {
namespace __private {

class PointerSet {
    PointerSet&operator=(const PointerSet &)=delete;
    PointerSet&operator=(PointerSet &&)=delete;
    PointerSet(const PointerSet &)=delete;
    PointerSet(PointerSet &&)=delete;
public:
    std::set<int> used;
    std::set<int> unused;
    PointerSet() {
#if defined(_DEBUG)
        //std::cout<<" pointer set "<<std::endl; 
#endif
    }
    ~PointerSet() {
#if defined(_DEBUG)
        //std::cout<<"~pointer set "<<std::endl; 
#endif
    }
    static constexpr const char * name() {
        return "PointerSet::pri::cct" __FILE__;
    }
};

class TopLock {
private:
    TopLock&operator=(TopLock&)=delete;
    TopLock&operator=(TopLock&&)=delete;
    TopLock(TopLock&)=delete;
    TopLock(TopLock&&)=delete;
    lua_State * L;
    int top;
public:
    TopLock(lua_State * L_):L(L_) { top=lua_gettop(L); }
    ~TopLock() { lua_settop(L,top); }
};

/*namespace*/
}
}
}

template<LuaValue::Type _Type_>
void LuaValue::_setValue(lua_State * L) {
    if (data_->type_!=_Type_) {
        clear();
        data_->type_=_Type_;
        __cct::__private::PointerSet * _ts_=
            reinterpret_cast<__cct::__private::PointerSet *>(getTypeSet(L,_Type_));

        if (_ts_->unused.empty()) {
            _ts_->unused.insert(_ts_->used.size());
        }

        auto pv_=_ts_->unused.begin();
        int ps_=*pv_;
        _ts_->unused.erase(pv_);
        _ts_->used.insert(ps_);

        data_->pointer_=ps_;
    }
}
template void LuaValue::_setValue<LuaValue::Type_Table>(lua_State * L);
template void LuaValue::_setValue<LuaValue::Type_Function>(lua_State * L);

void LuaValue::clear() {
    if (data_) {
        if (data_->type_==Type_NIL) { return; }
        std::shared_ptr<lua_State> ls_=data_->luaState_.lock();
        if (ls_) {
            lua_State * L=ls_.get();
            __cct::__private::TopLock topLock(L);

            __cct::__private::PointerSet * _ts_=
                reinterpret_cast<__cct::__private::PointerSet *>(getTypeSet(L,data_->type_));

            _ts_->used.erase(data_->pointer_);
            _ts_->unused.insert(data_->pointer_);

            lua_rawgetp(L,LUA_REGISTRYINDEX,getTablePathPointer(data_->type_));
            if (lua_istable(L,-1)) {
                lua_pushnil(L);
                lua_rawseti(L,-2,data_->pointer_);
            }
            data_->type_=Type_NIL;
        }
    }
}

LuaValue::~LuaValue() {
    clear();
}

LuaValue::LuaValue():data_(new Data_) {

}

QVariant LuaValue::getValue() const {
    return QVariant();
}

namespace {
namespace __cct {
namespace __private {

int delete_PointerSet(lua_State * _L) {
    void * data_0_=lua_touserdata(_L,-1);
    __cct::__private::PointerSet * data_1_=reinterpret_cast<__cct::__private::PointerSet *>(data_0_);
    data_1_->~PointerSet();
    return 0;
}

int error_function(lua_State * L) {

    if (lua_isstring(L,-1)) {
        std::cout<<"lua error:"<<lua_tostring(L,-1)<<std::endl;
        std::cout.flush();
        return 0;
    }

    if (luaL_callmeta(L,-1,"__tostring")&&
        (lua_type(L,-1)==LUA_TSTRING)) {
        std::cout<<"lua error:"<<lua_tostring(L,-1)<<std::endl;
        std::cout.flush();
        return 0;
    }

    std::cout<<"lua error:"<<"????"<<std::endl;
    std::cout.flush();
    return 0;
}

/*namespace*/
}
}
}

void LuaValue::operator()(int argc) {
    if (argc<0) { argc=0; }

    if (data_) {
        do {
            if (data_->type_!=Type_Function) { break; }

            auto ls__=data_->luaState_.lock();
            if (bool(ls__)==false) { break; }
            lua_State * L=ls__.get();

            lua_rawgetp(L,LUA_REGISTRYINDEX,getTablePathPointer(Type_Function));
            if (lua_istable(L,-1)) {
                lua_rawgeti(L,-1,data_->pointer_);
                if (lua_isfunction(L,-1)) {
                    if (argc==0) {
                        lua_pushcfunction(L,&__cct::__private::error_function);
                        lua_replace(L,-3);
                        lua_pcall(L,0,LUA_MULTRET,-2);
                    }
                    else {
                        const auto _argc_1=1+argc;
                        const auto _argc_2=_argc_1+1;
                        lua_copy(L,-1,-2); lua_pop(L,1);
                        lua_insert(L,lua_gettop(L)-_argc_1);/*set function to bottom*/
                        lua_pushcfunction(L,&__cct::__private::error_function);
                        lua_insert(L,lua_gettop(L)-_argc_2);/*set error function to bottom*/
                        lua_pcall(L,argc,LUA_MULTRET,lua_gettop(L)-_argc_2);
                    }
                }
            }
        } while (0);
    }
    return;
}

void* LuaValue::getTypeSet(lua_State * L,Type v) {

    lua_rawgetp(L,LUA_REGISTRYINDEX,getTypeSetPointer(v));
    if (lua_isuserdata(L,-1)==false) {

        void * data__=lua_newuserdata(L,sizeof(__cct::__private::PointerSet));
        new(data__) __cct::__private::PointerSet;

        luaL_getmetatable(L,__cct::__private::PointerSet::name());

        if (lua_istable(L,-1)==false) {

            lua_pop(L,1);
            luaL_newmetatable(L,__cct::__private::PointerSet::name());

            lua_pushlstring(L,"__gc",4);/*push key*/
            lua_pushcfunction(L,&__cct::__private::delete_PointerSet);/*push value*/
            lua_settable(L,-3);
        }

        lua_setmetatable(L,-2);

        lua_rawsetp(L,LUA_REGISTRYINDEX,getTypeSetPointer(v));
        return data__;
    }
    return lua_touserdata(L,-1);
}

void LuaValue::setFunction() {
    if (bool(data_)==false) { return; }
    auto ls__=data_->luaState_.lock();
    if (bool(ls__)==false) { return; }
    lua_State * L=ls__.get();
    __cct::__private::TopLock _lock_(L);
    if (lua_isfunction(L,-1)) {
        const int value_pos_=lua_gettop(L);
        _setValue<Type_Function>(L);
        lua_rawgetp(L,LUA_REGISTRYINDEX,getTablePathPointer(Type_Function));
        if (lua_istable(L,-1)==false) {
            lua_pop(L,1);
            lua_newtable(L);
            lua_pushvalue(L,-1);
            lua_rawsetp(L,LUA_REGISTRYINDEX,getTablePathPointer(Type_Function));
        }
        lua_pushvalue(L,value_pos_);
        lua_rawseti(L,-2,data_->pointer_);
    }
}

void LuaValue::setTable() {
    if (bool(data_)==false) { return; }
    auto ls__=data_->luaState_.lock();
    if (bool(ls__)==false) { return; }
    lua_State * L=ls__.get();
    __cct::__private::TopLock _lock_(L);
    if (lua_istable(L,-1)) {
        const int value_pos_=lua_gettop(L);
        _setValue<Type_Table>(L);
        lua_rawgetp(L,LUA_REGISTRYINDEX,getTablePathPointer(Type_Table));
        if (lua_istable(L,-1)==false) {
            lua_pop(L,1);
            lua_newtable(L);
            lua_pushvalue(L,-1);
            lua_rawsetp(L,LUA_REGISTRYINDEX,getTablePathPointer(Type_Table));
        }
        lua_pushvalue(L,value_pos_);
        lua_rawseti(L,-2,data_->pointer_);
    }
}

void LuaValue::pushValue() const {
    if (bool(data_)==false) { return; }
    auto ls__=data_->luaState_.lock();
    if (bool(ls__)==false) { return; }
    lua_State * L=ls__.get();

    if (data_->type_==Type_NIL) {
        lua_pushnil(L);
        return;
    }

    lua_rawgetp(L,LUA_REGISTRYINDEX,getTablePathPointer(data_->type_));
    if (lua_istable(L,-1)==false) {
        lua_pop(L,-1);
        lua_pushnil(L);
        return;
    }
    else {
        lua_rawgeti(L,-1,data_->pointer_);
        lua_copy(L,-1,-2);
        lua_pop(L,1);
        return;
    }

}

namespace {
namespace __cct {
namespace __private {

/* input a table and out put a table */
inline int luaL_copyTable(lua_State * L) {

    if (lua_istable(L,-1)) {
        const auto source_table_top_=lua_gettop(L);

        std::list<int> from_,to_;
        from_.push_back(source_table_top_);
        std::map< const void *,int > all_tables_;

        lua_newtable(L);/*create temp table*/
        const auto temp_table_=lua_gettop(L);

        lua_pushnil(L);
        lua_copy(L,-2,-1);/*copy soure table*/

        lua_newtable(L);
        const auto target_table_=lua_gettop(L);
        to_.push_back(target_table_);/*create target table*/

        lua_pushnil(L);
        lua_pushnil(L);
        lua_copy(L,-4,-2);/*copy soure table*/
        lua_copy(L,-3,-1);/*copy target table*/

        int copy_table_count_=0;

    copy_next_table:
        while (from_.empty()==false) {
            const auto tf_=*from_.crbegin();
            const auto tt_=*to_.crbegin();
            from_.pop_back(); to_.pop_back();

            if (lua_istable(L,-1)) {
                lua_pushnil(L);
            }

            while (lua_next(L,tf_)!=0) {
                /*-1 value -2 key*/
                if (lua_istable(L,-1)) {
                    const void * table_pointer=lua_topointer(L,-1);
                    auto end_=all_tables_.end();
                    auto pos_=all_tables_.find(table_pointer);
                    if (end_!=pos_) {
                        /* copy value has exist */
                        lua_pop(L,1);/*remove old one*/
                        const auto ttt_=pos_->second;
                        lua_pushinteger(L,ttt_);
                        lua_gettable(L,temp_table_);/*set value*/
                        lua_pushnil(L); lua_pushnil(L);
                        lua_copy(L,-3,-1); lua_copy(L,-4,-2);
                        lua_settable(L,tt_);
                        lua_pop(L,1);/*remove the table*/
                    }
                    else {
                        /* copy value not exist */
                        ++copy_table_count_;

                        all_tables_[table_pointer]=copy_table_count_;

                        lua_pushnil(L);
                        lua_pushinteger(L,copy_table_count_);
                        lua_newtable(L);
                        lua_copy(L,-1,-3);

                        /*set temp table*/
                        lua_settable(L,temp_table_);

                        /*set current table to temp table*/
                        lua_pushnil(L); lua_pushnil(L);
                        lua_copy(L,-3,-1); lua_copy(L,-5,-2);
                        lua_settable(L,tt_);

                        from_.push_back(tf_);
                        to_.push_back(tt_);
                        auto ttt_=lua_gettop(L);
                        to_.push_back(ttt_);
                        from_.push_back(ttt_-1);

                        goto copy_next_table;
                    }
                }
                else {
                    /* copy value not table */
                    lua_pushnil(L); lua_pushnil(L);
                    lua_copy(L,-3,-1); lua_copy(L,-4,-2);
                    lua_settable(L,tt_);/*remove key value*/
                    lua_pop(L,1);/*remove key*/
                }
            }/*~while*/

            lua_pop(L,2);/*remove source table , target table*/

        }/*all while*/

         /* return ans */
        lua_copy(L,target_table_,source_table_top_);
        lua_settop(L,source_table_top_);
        return 1;

    }/*~if*/

    /*确保输出一个table*/
    lua_newtable(L);
    return 1;
}

/*namespace*/
}
}
}

namespace {
namespace __cct {
namespace __private {

inline void luaL_printTable(
    lua_State * L,
    std::function<void(const std::string &)> print_
    ) {

    /*suport type*/
    enum DataType : int {
        Bool,Number,Integer,String,NIL
    };

    class Data {
    public:
        Data():type(NIL) {}
        Data(bool i):type(Bool),boolean(i) {}
        Data(lua_Integer i):type(Integer),integer(i) {}
        Data(lua_Number i):type(Number),number(i) {}
        Data(const std::string & i):type(String),string(i) {}
        Data(std::string && i):type(String),string(std::move(i)) {}
        Data(const Data & v):type(v.type) {
            switch (type) {
                case Bool:boolean=v.boolean; break;
                case Number:number=v.number; break;
                case Integer:integer=v.integer; break;
                case String: {new(&string) std::string(v.string); }; break;
                case NIL:; break;
            }
        }

        Data(Data && v):type(v.type) {
            switch (type) {
                case Bool:boolean=v.boolean; break;
                case Number:number=v.number; break;
                case Integer:integer=v.integer; break;
                case String: {new(&string) std::string(std::move(v.string)); }; break;
                case NIL:; break;
            }
        }

        Data & operator=(const Data & v) {
            if (this==&v) { return *this; }
            if (v.type==String) {
                if (type==String) { string=v.string; }
                else {
                    type=String;
                    new (&string) std::string(v.string);
                }
            }
            else {
                if (type==String) {
                    typedef std::string _T_;
                    string.~_T_();
                }
                switch (v.type) {
                    case Bool: {type=Bool; boolean=v.boolean; } break;
                    case Number: {type=Number; number=v.number; }break;
                    case Integer: {type=Integer; integer=v.integer; }break;
                    case String: {}break;
                    case NIL: {type=NIL; }break;
                }
            }

            return *this;
        }

        Data & operator=(Data && v) {
            if (this==&v) { return *this; }
            if (v.type==String) {
                if (type==String) { string=std::move(v.string); }
                else {
                    type=String;
                    new (&string) std::string(std::move(v.string));
                }
            }
            else {
                if (type==String) {
                    typedef std::string _T_;
                    string.~_T_();
                }
                switch (v.type) {
                    case Bool: {type=Bool; boolean=v.boolean; }break;
                    case Number: {type=Number; number=v.number; }break;
                    case Integer: {type=Integer; integer=v.integer; }break;
                    case String: {}break;
                    case NIL: {type=NIL; }break;
                }
            }

            return *this;
        }

        ~Data() {
            if (type==String) { typedef std::string _T_; string.~_T_(); }
        }

        DataType type;
        union {
            bool boolean;
            lua_Number number;
            lua_Integer integer;
            std::string string;
        };
    };

    struct TableMemory {
        Data data;
        int table;
        bool ipair=false;
        lua_Integer ipair_index=1;
        lua_Integer tableIndex=0;
        TableMemory() {}
        TableMemory(const std::string & v,int t):data(v),table(t) {}
        TableMemory(std::string && v,int t):data(std::move(v)),table(t) {}
        TableMemory(std::string && v,int t,bool i,lua_Integer ii,lua_Integer ti):data(std::move(v)),table(t),ipair(i),ipair_index(ii),tableIndex(ti) {}
        TableMemory(Data && v,int t):data(std::move(v)),table(t) {}
        TableMemory(const Data & v,int t):data(v),table(t) {}
        TableMemory(Data && v,int t,bool i,lua_Integer ii,lua_Integer ti):data(std::move(v)),table(t),ipair(i),ipair_index(ii),tableIndex(ti) {}
    };

    const auto is_integer=[](const std::string & v)->bool {
        if (v.empty()) { return false; }
        const auto i=v[0]; if ((i>='0')&&(i<='9')) { return true; }
        return false;
    };

    const auto to_string=[](const auto & i) {
        std::stringstream ss; ss.precision(FLOAT_PRECISION_);
        ss<<i; std::string ans; ss>>ans; return std::move(ans);
    };

    const auto oprint_key=[&print_,&to_string,&is_integer](const Data & v) {
        const int & type=v.type;
        switch (type) {
            case DataType::Number: { print_("["); print_(to_string(v.number)); print_("]"); }; break;
            case DataType::Integer: { print_("["); print_(to_string(v.integer)); print_("]"); }; break;
            case DataType::String:
                if (is_integer(v.string)) {
                    print_("[\""); print_(v.string); print_("\"]");
                }
                else {
                    print_(v.string);
                }break;
        }
    };

    const auto string_find=[](const std::string & s,const auto & v) {return (s.find(v)!=s.npos); };

    const auto oprint_value=[&print_,&to_string,&string_find](const Data & v) {
        const DataType & type=v.type;
        switch (type) {
            case DataType::Bool: { if (v.boolean) { print_("true"); } else { print_("false"); } }break;
            case DataType::Number: {  print_(to_string(v.number));  }; break;
            case DataType::Integer: { print_(to_string(v.integer));  }; break;
            case DataType::String: {
                std::string op_="==";
                while ((string_find(v.string,"["+op_+"["))||string_find(v.string,"]"+op_+"]")) { op_+="==="; }
                print_("["+op_+"[");
                print_(v.string);
                print_("]"+op_+"]");
            }; break;
            case DataType::NIL: {print_("nil"); }  break;
        }
    };

    const auto print_endl=[&print_]() {print_("\n"); };
    const auto print_line_endl=[&print_]() {print_(" ;  \n"); };
    const auto print_file_begin=[&print_](const std::string & tbname) {
        print_(tbname);
        print_(" = (function()                    \n\nlocal ");
    };

    const auto print_file_end=[&print_](const std::string & tbname) {
        print_("\nreturn "+tbname);
        print_("; end)()");
    };

    const auto table_name_full=[&to_string](const std::list< TableMemory > & tables,const Data & ctable)->std::string {
        auto root_name_=tables.begin();
        auto end_name_=tables.end();
        std::string ans;
        ans=root_name_->data.string;
        for (auto next_name_=root_name_; (++next_name_)!=end_name_; ) {
            const auto &current_table_name=(*next_name_).data;
            switch (int(current_table_name.type)) {
                case DataType::Integer: ans+=std::move("["+to_string(current_table_name.integer)+"]"); break;
                case DataType::Number: ans+=std::move("["+to_string(current_table_name.number)+"]"); ; break;
                case DataType::String: ans+=std::move("[\""+current_table_name.string+"\"]"); break;
            }
        }
        const auto &current_table_name=ctable;
        switch (int(current_table_name.type)) {
            case DataType::Integer: ans+=std::move("["+to_string(current_table_name.integer)+"]"); break;
            case DataType::Number: ans+=std::move("["+to_string(current_table_name.number)+"]"); ; break;
            case DataType::String: ans+=std::move("[\""+current_table_name.string+"\"]"); break;
        }
        return std::move(ans);
    };

    /*-----------------------------------------------*/
    if (lua_gettop(L)<2) { lua_pushstring(L,"you must input table name and table"); lua_error(L); }
    if (lua_isstring(L,1)==false) { lua_pushstring(L,"table name must a string"); lua_error(L); }
    if (lua_istable(L,2)==false) { lua_pushstring(L,"you must input a table"); lua_error(L); }

    /* copy the table twice */
    lua_pushvalue(L,2);
    lua_pushvalue(L,2);

    const std::string tbname(lua_tostring(L,1));
    print_file_begin(tbname);

    /*-----------------------------------------------*/
    lua_Integer table_index_=0;
    std::list< TableMemory > print_tables{ {tbname, 2} };
    std::list< std::string > final_print_lines;
    std::map< const void *,std::string > all_tables{ {lua_topointer(L,2),tbname} };

print_next_start:
    while (print_tables.empty()==false) {

        auto & current_table_memory=*print_tables.rbegin();
        int current_table=current_table_memory.table;/*the table in the lua state*/
        Data current_table_name=std::move(current_table_memory.data);/*the table name */
        bool ipair_=current_table_memory.ipair;/*the value will be true if the index follow 1,2,3....  */
        lua_Integer ipair_index_=current_table_memory.ipair_index;/*current index if ipair is true*/
        lua_Integer current_table_index=current_table_memory.tableIndex;/*the table index in this function */

        print_tables.pop_back();

        /* reset the value : ipair_ */
        if (lua_istable(L,-1)) {

            const static constexpr char start_string[]{ "{   " };
            switch (int(current_table_name.type)) {
                case DataType::Integer:if (false==ipair_) { print_("["+to_string(current_table_name.integer)+"] ="); }print_(start_string); break;
                case DataType::Number: if (false==ipair_) { print_("["+to_string(current_table_name.number)+"] ="); }print_(start_string); break;
                case DataType::String: {
                    if (is_integer(current_table_name.string)) {
                        print_("[\""+current_table_name.string+"\"] ="+start_string);
                    }
                    else {
                        print_(current_table_name.string);
                        print_(" =");
                        print_(start_string);
                    }
                } break;
            }

            lua_pushnil(L);
            ipair_=true;
            print_("--[[ "+to_string(current_table_index)+" --]]  \n");

        }

        while (lua_next(L,current_table)!=0) {
            /* uses 'key' (at index -2) and 'value' (at index -1) */
            if (lua_istable(L,-1)) {

                Data key_;
                auto type_=lua_type(L,-2);
                switch (type_) {
                    case LUA_TSTRING:ipair_=false; key_=std::string(lua_tostring(L,-2));  break;
                    case LUA_TNUMBER: {
                        key_=lua_tointeger(L,-2);
                        if (ipair_) {
                            if (key_.integer==ipair_index_) {
                                ++ipair_index_;
                            }
                            else {
                                ipair_=false;
                            }
                        } } break;
                    default: lua_pushstring(L,"key must be number or string"); lua_error(L); ; break;
                }

                const void * table_pointer=lua_topointer(L,-1);
                auto ctable=all_tables.find(table_pointer);
                const auto etable=all_tables.end();

                if (ctable==etable) {
                    /*find a new table*/
                    ++table_index_;
                    /*save old value*/
                    print_tables.emplace_back(std::move(current_table_name),current_table,ipair_,ipair_index_,current_table_index);

                    all_tables.insert({ table_pointer,table_name_full(print_tables ,key_) });

                    current_table=lua_gettop(L);
                    current_table_name=std::move(key_);

                    /*set new value*/
                    print_tables.emplace_back(std::move(current_table_name),current_table,ipair_,1,table_index_);

                    goto print_next_start;
                }
                else {
                    /*circle table*/

                    lua_pop(L,1);/*pop the table*/
                    ipair_=false;/*circle table do not surport ipair*/

                    print_tables.emplace_back(std::move(current_table_name),-1);
                    auto this_table_name_=table_name_full(print_tables,key_);

                    current_table_name=std::move((*print_tables.rbegin()).data);
                    print_tables.pop_back();

                    final_print_lines.push_back(std::move(this_table_name_)+" = "+ctable->second+"\n");
                }

            }
            else {
                Data key_,value_;
                bool do_not_support=false;
                auto type_=lua_type(L,-2);
                switch (type_) {
                    case LUA_TSTRING: ipair_=false; key_=std::string(lua_tostring(L,-2));  break;
                    case LUA_TNUMBER: {key_=lua_tointeger(L,-2);
                        if (ipair_) {
                            if (key_.integer==ipair_index_) {
                                ++ipair_index_;
                            }
                            else {
                                ipair_=false;
                            }
                        }
                    } break;
                    default: lua_pushstring(L,"key must be number or string"); lua_error(L); ; break;
                }

                type_=lua_type(L,-1);
                switch (type_) {
                    case LUA_TNIL: break;
                    case LUA_TBOOLEAN: value_=bool(lua_toboolean(L,-1)); break;
                    case LUA_TSTRING: value_=std::string(lua_tostring(L,-1)); break;
                    case LUA_TNUMBER: value_=lua_tonumber(L,-1); break;
                    case LUA_TFUNCTION: value_=std::string("function() end"); break;
                    default: do_not_support=true; break;
                }

                if (do_not_support) { lua_pop(L,1); continue; }

                if (false==ipair_) {
                    oprint_key(key_);
                    print_(" = ");
                }
                oprint_value(value_);
                print_line_endl();

                lua_pop(L,1);
            }
        }/*while table*/

        if (print_tables.empty()) {
            print_("} --[[ "+to_string(current_table_index)+" --]] \n");
        }
        else {
            print_("} , --[[ "+to_string(current_table_index)+" --]] \n");
        }

        lua_pop(L,1);
    }

    if (false==final_print_lines.empty()) {
        print_endl();
        for (const auto & line:final_print_lines) {
            print_(line);
        }
    }

    print_file_end(tbname);

    print_endl();
    print_("--[[***********************--]]\n");
    print_endl();

    return;
}

/*namespace*/
}
}
}

int LuaUtility::copyTable(lua_State *L) {
    if (L==nullptr) { return 0; }
    return __cct::__private::luaL_copyTable(L);
}

void LuaUtility::printTable(
        lua_State *L,
        std::function<void(const std::string &)> print_) {
    return __cct::__private::luaL_printTable(L,std::move(print_));
}

int LuaUtility::showTable(lua_State * L) {
    if (L==nullptr) { return 0; }
    printTable(L,[](const std::string & str) {
        std::cout<<str;
    });
    std::cout.flush();
    return 0;
}

int LuaUtility::tableToString(lua_State * L) {
    if (L==nullptr) { return 0; }
    std::list<std::string> tmp_;
    std::string::size_type length_=0;
    printTable(L,[&tmp_,&length_](const std::string & str) {
        length_+=(str.size()+1);
        tmp_.push_back(str);
    });

    {
        std::unique_ptr<luaL_Buffer> buffer__(new luaL_Buffer);
        register luaL_Buffer & buffer_=*buffer__;
        luaL_buffinitsize(L,&buffer_,length_);
        while (tmp_.empty()==false) {
            std::string str=std::move(*tmp_.begin());
            tmp_.pop_front();
            luaL_addlstring(&buffer_,str.c_str(),str.size());
        }
        luaL_pushresult(&buffer_);
    }

    return 1;
}

int LuaUtility::openLib(lua_State * L) {
    constexpr const static struct luaL_Reg libs_[]{
        {"copyTable",&LuaUtility::copyTable},
        {"showTable",&LuaUtility::showTable},
        {"tableToString",&LuaUtility::tableToString},
        {nullptr,nullptr}
    };

    luaL_newlib(L,libs_);

    return 1;
}

void LuaUtility::loadModule(lua_State * L) {
    luaL_requiref(L,"utility",&LuaUtility::openLib,1);
    lua_pop(L,1);  /* remove lib */
    return;
}

/*
 * 扩展LUA
 * 此文件应当采用utf8 with bom 编码
 * Final
*/


