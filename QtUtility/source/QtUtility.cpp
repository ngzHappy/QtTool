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

QtUtility::QtUtilityPrivateStaticData_ * QtUtility::staticData_ = nullptr ;

class QtUtility::QtUtilityPrivateStaticData_{
public:
    std::forward_list<ConstructFunctionType> constructFunctions;
    std::forward_list<DestructFunctionType> destructFunctions;
    std::thread::id mainThreadID;
    QtUtilityPrivateStaticData_(){
        mainThreadID=std::this_thread::get_id();
    }
};

#if defined(QT_CORE_LIB)
namespace  {
/*
 * add destruct function
 * when QCoreApplication quit call it
 * 当QCoreApplication退出的时候调用此模块的析构函数
*/
bool _static_initThisModule=[](){
    std::srand( int(std::time(0)) );
    QtUtility::construct();
    qAddPostRoutine(&QtUtility::destruct);
    return bool(std::rand()&1);
}();
}
#endif

QtUtility::QtUtility(){
}

QtUtility::~QtUtility(){

}

void QtUtility::addConstructFunction(QtUtility::ConstructFunctionType f){
    staticData().constructFunctions.push_front(f);
}
void QtUtility::addDestructFunction(QtUtility::DestructFunctionType f){
    staticData().destructFunctions.push_front(f);
}
void QtUtility::construct(){
    auto & cf_ = staticData().constructFunctions;
    auto b = cf_.begin() ;
    while ( b!=cf_.end() ) {
        QtUtility::ConstructFunctionType f = *b;
        cf_.pop_front();
        b = cf_.begin();
        if(f){f();}
    }
}
void QtUtility::destruct(){
    auto & cf_ = staticData().destructFunctions;
    auto b = cf_.begin() ;
    while ( (staticData_)&&(b!=cf_.end()) ) {
        QtUtility::DestructFunctionType f = *b;
        cf_.pop_front();
        b = cf_.begin();
        if(f){f();}
    }
}

std::thread::id QtUtility::mainThread() {
    return staticData().mainThreadID;
}

QtUtility::QtUtilityPrivateStaticData & QtUtility::staticData(){
    if(staticData_){return *staticData_;}
    staticData_ = new QtUtilityPrivateStaticData_;
    staticData_->destructFunctions.push_front( [](){ delete staticData_;staticData_=nullptr; } );
    return *staticData_;
}


/*
 * 此文件应当采用UTF8 with bom的方式编码
*/

