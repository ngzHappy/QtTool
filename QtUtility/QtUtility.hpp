#if !defined(_0_QTUTILITY_HPP)
#define _0_QTUTILITY_HPP

#include "qtutility_global.hpp"
#include <QtCore/qcoreapplication.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qvariant.h>

#include <limits>
#include <ciso646>
#include <memory>
#include <thread>
#include <cstddef>
#include <cstdlib>
#include <utility>
#include <algorithm>
#include <functional>
#include <type_traits>

#include <set>
#include <map>
#include <list>
#include <deque>
#include <vector>
#include <string>
#include <forward_list>

#include "quazip/quazip.h"
#include "quazip/quazipdir.h"
#include "quazip/quazipdir.h"
#include "lua/lua.hpp"

#if defined(_DEBUG)
#include "lua/lstate.h"
#endif

#include <QtCore/qobject.h>
#include <QtCore/qthread.h>

class QTUTILITYSHARED_EXPORT QtUtility : public QObject {
    Q_OBJECT
public:
    class QtUtilityPrivateStaticData_;
    typedef QtUtility::QtUtilityPrivateStaticData_ QtUtilityPrivateStaticData;
private:
    static QtUtility::QtUtilityPrivateStaticData_ * staticData_;
public:

    typedef void(*FunctionType)(void);
    typedef FunctionType ConstructFunctionType;
    typedef FunctionType DestructFunctionType;

    static QtUtilityPrivateStaticData & staticData();

    /*construct and destruct static data in this module*/
    static void addConstructFunction(ConstructFunctionType);
    static void addDestructFunction(DestructFunctionType);
    static void construct();
    static void destruct();
    static QtUtility * instance();

    static QThread * mainQThread() { return qApp->thread(); }
    static std::thread::id mainThread();
protected:
    QtUtility();
    virtual ~QtUtility();
};

class QTUTILITYSHARED_EXPORT KeyUnion {
public:
    enum class Type :int {
        Type_NIL,
        Type_String,
        Type_Float,
        Type_Int,
    };
    typedef double Float;
    typedef int Int;
private:
    union Data_ {
        QByteArray utf8Data;
        Int intData;
        Float floatData;
        Data_() {}
        ~Data_() {}
    };
    Type type_=Type::Type_NIL;
    Data_ data_;
public:

    Type getType() const { return type_; }
    Float getFloat() const { return data_.floatData; }
    Int getInt() const { return data_.intData; }
    const QByteArray &getUTF8() const { return data_.utf8Data; }

    QByteArray toUTF8()const;
    Int toInt(bool * /**/=nullptr)const;
    Float toFloat(bool * /**/=nullptr)const;

    KeyUnion();
    KeyUnion(decltype(nullptr)):KeyUnion() {}
    KeyUnion(const char * v) { setValue(QByteArray(v)); }
    KeyUnion(const char * v,int l) { setValue(v,l); }
    KeyUnion(const QString & v) { setValue(v); }
    KeyUnion(const std::string & v) { setValue(v); }
    ~KeyUnion();

    KeyUnion(Float);
    KeyUnion(Int);
    KeyUnion(QByteArray);

    KeyUnion(const KeyUnion &);
    KeyUnion(KeyUnion &&);
    KeyUnion&operator=(const KeyUnion &);
    KeyUnion&operator=(KeyUnion &&);

    void setValue(Float);
    void setValue(Int);
    void setValue(QByteArray &&);
    void setValue(const QByteArray &);
    void setValue(decltype(nullptr)) { clear(); }
    void setValue(const char * v,int l) { setValue(QByteArray(v,l)); }
    void setValue(const QString & v) { setValue(v.toUtf8()); }
    void setValue(const std::string & v) { setValue(QByteArray::fromStdString(v)); }

    void clear();
};

#include "ReadOnly.hpp"
#include "LUAUtiltiy.hpp"
#include "QtTextDocumentLayout.hpp"

#endif // QTUTILITY_HPP

/*
 * 本模块包含QUAZIP LUA
 * 注意:LUA协程模块已经被删除!!
*/

