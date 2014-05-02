#ifndef SFP_CFUNC_HPP_
#define SFP_CFUNC_HPP_

//@PURPOSE: Provide metafunction for generating curried Boost.Function types.
//
//@CLASSES:
//  sfp::cfunc: curried Boost.Function generating metafunction
//
//@DESCRIPTION: This component provides a single class, 'cfunc', which based on
// its template argments, will produce a curried Boost.Function type.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Generation of Boost.Function types
// - - - - - - - - - - - - - - - - - - - - - - -
// Each template argument to 'cfunc' corresponds to a parameter of the function
// with the final argument representing the result type.
//
// For example, the following invocation of 'cfunc',
//..
//  sfp::cfunc<char,int,long>::type
//..
// evaluates to
//..
//  boost::function< boost::function< long (int)>
//                   (char)
//                 >
//..

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/void.hpp>
#include <boost/function.hpp>

namespace sfp {

#ifndef CFUNC_MAX_SIZE
#define CFUNC_MAX_SIZE 10
#endif

#define CFUNC_PRINT(z, n, data) data

#define CFUNC_CR(z, n, data) BOOST_PP_CAT(data, n) = boost::mpl::void_

template< BOOST_PP_ENUM( BOOST_PP_ADD( CFUNC_MAX_SIZE, 1), CFUNC_CR, class T) > 
struct cfunc;

//base case
template< typename A, typename B >
struct cfunc< A
            , B
            BOOST_PP_COMMA_IF( BOOST_PP_GREATER( CFUNC_MAX_SIZE, 2 ) )
            BOOST_PP_ENUM( BOOST_PP_SUB( CFUNC_MAX_SIZE, 1 )
                         , CFUNC_PRINT
                         , boost::mpl::void_
                         )
            >
{
    typedef boost::function< B (A) > type;
};


#define BOOST_PP_ITERATION_LIMITS (3, CFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/cfunc_spec.hpp"
#include BOOST_PP_ITERATE()

#undef CFUNC_CR
#undef CFUNC_PRINT

}
#endif
