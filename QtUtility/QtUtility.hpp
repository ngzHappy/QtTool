﻿#if !defined(_0_QTUTILITY_HPP)
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

#include "ReadOnly.hpp"
#include "LUAUtiltiy.hpp"

#endif // QTUTILITY_HPP

/*
 * 本模块包含QUAZIP LUA
 * 注意:LUA协程模块已经被删除!!
*/

