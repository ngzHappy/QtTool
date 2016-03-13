#if  !defined(_0_READONLY_HPP)
#define _0_READONLY_HPP

#include "QtUtility.hpp"

template<typename _T_,typename _U_=void,int _TypeIndex_=0>
const _T_ & readOnly( _T_ & value_ ){ return value_; }



#endif // READONLY_HPP
