#include "QtUtility.hpp"
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <ctime>

/*build all qt headers*/
#if defined(QT_CORE_LIB)
#include <QtCore/QtCore>
#endif

#if defined(QT_GUI_LIB)
#include <QtGui/QtGui>
#endif

#if defined(QT_WIDGETS_LIB)
#include <QtWidgets/QtWidgets>
#endif

/*build all quazip headers*/
#include <quazip/quazip.h>

QtUtility::QtUtilityPrivateStaticData_ * QtUtility::staticData_=nullptr;

class QtUtility::QtUtilityPrivateStaticData_ {
public:
    QtUtility * const thisModule;
    std::forward_list<ConstructFunctionType> constructFunctions;
    std::forward_list<DestructFunctionType> destructFunctions;
    std::thread::id mainThreadID;
    QtUtilityPrivateStaticData_():thisModule(new QtUtility) {
        mainThreadID=std::this_thread::get_id();
    }
    ~QtUtilityPrivateStaticData_() {
        delete thisModule;
    }
};

#if defined(QT_CORE_LIB)
namespace {
/*
 * add destruct function
 * when QCoreApplication quit call it
 * 当QCoreApplication退出的时候调用此模块的析构函数
*/
bool _static_initThisModule=[]() {
    std::srand(int(std::time(0)));
    QtUtility::construct();
    qAddPostRoutine(&QtUtility::destruct);
    return bool(std::rand()&1);
}();
}
#endif

QtUtility::QtUtility() {
    if (_static_initThisModule) {
        /*update rand*/
        _static_initThisModule=bool(std::rand()&1);
    }
}

QtUtility::~QtUtility() {

}

void QtUtility::addConstructFunction(QtUtility::ConstructFunctionType f) {
    staticData().constructFunctions.push_front(f);
}
void QtUtility::addDestructFunction(QtUtility::DestructFunctionType f) {
    staticData().destructFunctions.push_front(f);
}
void QtUtility::construct() {
    auto & cf_=staticData().constructFunctions;
    auto b=cf_.begin();
    while (b!=cf_.end()) {
        QtUtility::ConstructFunctionType f=*b;
        cf_.pop_front();
        b=cf_.begin();
        if (f) { f(); }
    }
}
void QtUtility::destruct() {
    auto & cf_=staticData().destructFunctions;
    auto b=cf_.begin();
    while ((staticData_)&&(b!=cf_.end())) {
        QtUtility::DestructFunctionType f=*b;
        cf_.pop_front();
        b=cf_.begin();
        if (f) { f(); }
    }
}

std::thread::id QtUtility::mainThread() {
    return staticData().mainThreadID;
}

QtUtility::QtUtilityPrivateStaticData & QtUtility::staticData() {
    if (staticData_) { return *staticData_; }
    staticData_=new QtUtilityPrivateStaticData_;
    staticData_->destructFunctions.push_front([]() { delete staticData_; staticData_=nullptr; });
    return *staticData_;
}

QtUtility * QtUtility::instance() {
    return staticData().thisModule;
}

KeyUnion::KeyUnion() {}
KeyUnion::~KeyUnion() { clear(); }

KeyUnion::KeyUnion(Float v) { setValue(v); }
KeyUnion::KeyUnion(Int v) { setValue(v); }
KeyUnion::KeyUnion(QByteArray v) { setValue(std::move(v)); }

KeyUnion::KeyUnion(const KeyUnion & v) {
    (*this)=v;
}

KeyUnion::KeyUnion(KeyUnion && v) {
    (*this)=std::move(v);
}

KeyUnion&KeyUnion::operator=(const KeyUnion & v) {
    if (this==&v) { return *this; }
    switch (v.type_) {
        case Type::Type_NIL:clear(); break;
        case Type::Type_String:setValue(v.data_.utf8Data);  break;
        case Type::Type_Float:setValue(v.data_.floatData); break;
        case Type::Type_Int:setValue(v.data_.intData); break;
    }
    return *this;
}

KeyUnion&KeyUnion::operator=(KeyUnion && v) {
    if (this==&v) { return *this; }
    switch (v.type_) {
        case Type::Type_NIL:clear(); break;
        case Type::Type_String:setValue(std::move(v.data_.utf8Data)); break;
        case Type::Type_Float:setValue(v.data_.floatData); break;
        case Type::Type_Int:setValue(v.data_.intData); break;
    }
    return *this;
}

void KeyUnion::clear() {
    if (type_==Type::Type_String) { data_.utf8Data.~QByteArray(); }
    type_=Type::Type_NIL;
}

void KeyUnion::setValue(Float v) {
    clear(); type_=Type::Type_Float; data_.floatData=v;
}
void KeyUnion::setValue(Int v) {
    clear(); type_=Type::Type_Int; data_.intData=v;
}

QByteArray KeyUnion::toUTF8()const {
    switch (type_) {
        case KeyUnion::Type::Type_NIL:return "nil"; break;
        case KeyUnion::Type::Type_String:return data_.utf8Data; break;
        case KeyUnion::Type::Type_Float:return QByteArray::number(data_.floatData); break;
        case KeyUnion::Type::Type_Int:return QByteArray::number(data_.intData); break;
        default:break;
    }
    return "nil";
}

KeyUnion::Int KeyUnion::toInt(bool * v)const {
    bool isOK_=false; if (v==nullptr) { v=&isOK_; }
    switch (type_) {
        case KeyUnion::Type::Type_NIL: *v=false; break;
        case KeyUnion::Type::Type_String: {
            auto ans_=data_.utf8Data.toLongLong(v);
            if (*v) {
                if (std::is_same<Int,long long>::value==false) {
                    if (
                        (ans_<=std::numeric_limits<Int>::max())&&
                        (ans_>=std::numeric_limits<Int>::lowest())
                        ) {
                        return static_cast<Int>(ans_);
                    }
                }
                else { return ans_; }
            }
        } break;
        case KeyUnion::Type::Type_Float: {
            if (
                (data_.floatData<=std::numeric_limits<Int>::max())&&
                (data_.floatData>=std::numeric_limits<Int>::lowest())
                ) {
                *v=true;
                return static_cast<Int>(data_.floatData);
            }
        }break;
        case KeyUnion::Type::Type_Int: *v=true; return data_.intData; break;
        default:break;
    }
    *v=false;
    return 0;
}

KeyUnion::Float KeyUnion::toFloat(bool * v)const {
    bool isOK_=false; if (v==nullptr) { v=&isOK_; }
    constexpr auto max_=std::numeric_limits<Float>::max();
    constexpr auto min_=std::numeric_limits<Float>::lowest();
    switch (type_) {
        case KeyUnion::Type::Type_NIL:*v=false; break;
        case KeyUnion::Type::Type_String: {
            auto ans_=data_.utf8Data.toDouble(v);
            if (*v) {
                if (std::is_same< decltype(ans_),Float >::value==false) {
                    if (ans_>max_) { *v=false; return 0; }
                    if (ans_<min_) { *v=false; return 0; }
                    return ans_;
                }
                else { return ans_; }
            };
        }break;
        case KeyUnion::Type::Type_Float:*v=true; return data_.floatData; break;
        case KeyUnion::Type::Type_Int:*v=true; return data_.intData;; break;
        default:break;
    }
    *v=false; return 0;
}

void KeyUnion::setValue(const QByteArray & v) {
    if (type_==Type::Type_String) {
        data_.utf8Data=v;
        return;
    }
    {
        void * data__=&data_.utf8Data;
        new (data__) QByteArray(v);
        type_=Type::Type_String;
        return;
    }
}

void KeyUnion::setValue(QByteArray && v) {
    if (type_==Type::Type_String) {
        data_.utf8Data=std::move(v);
        return;
    }
    {
        void * data__=&data_.utf8Data;
        new (data__) QByteArray(std::move(v));
        type_=Type::Type_String;
        return;
    }
}

/*
 * 此文件应当采用UTF8 with bom的方式编码
 * Final file
*/

