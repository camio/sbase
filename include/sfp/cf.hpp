#ifndef SFP_CF_HPP_
#define SFP_CF_HPP_

//@PURPOSE: Provide routines that make curried versions of functions. 
//
//@FUNCTIONS:
//  sfp::cf: function to curried boost::function converter
//  sfp::cfi: function to curried functor converter
//  
//@SEE_ALSO: sfp_ciof
//
//@DESCRIPTION: This component provides a means to convert function pointers
// into curried variants.
//
// Given a function 'f' with 'i' arguments,
//..
//  R f( A₁, A₂, …, Aᵢ );
//..
//
// , 'cfi(f)' or 'cf(f)' is a curried form of 'f'.
//
// A value of type 'R' can be computed with 'cfi(f)(a₁)(a₂)⋯(aᵢ)' where
// 'aᵣ:Aᵣ'.
//
// Argument Copies
// ---------------
//
// Copies are always made of aᵣ where r < i. This is sometimes unwanted for
// either efficience concerns or when the datatype is noncopyable. To overcome
// this limitation, boost::ref may be used on arguments that should be passed
// by reference. See the following example:
//..
//  using boost::ref;
//  using gfp::cfi;
//
//  //Noncopyable object
//  struct NCO : boost::noncopyable { };
//  void f₀( NCO &, int i ) { }
//  void f₁( int i, NCO & ) { }
//
//  void g_cfi()
//  { NCO nco
//  ; cfi( f₀ )(      nco   )( i ) // Doesn't compile, attempts copy
//  ; cfi( f₀ )( ref( nco ) )( i ) // Correctly uses reference
//  ; cfi( f₁ )( i )( nco )        // Correctly uses reference. The last
//                                 // argument doesn't need a ref annotation.
//  }
//..
//
// Differences between 'cf' and 'cfi'
// ----------------------------------
//
// The main difference between 'cf' (curried function) and 'cfi' (curried
// function inline) is that the latter doesn't return a boost function and
// therefore incurs no allocation and calls are inline.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Using cfi on a function
// - - - - - - - - - - - - - - - - -
//
// 'cfi' allows the compiler to inline curried function calls. For example,
// given a function declaration as follows:
//..
//  int doIt( int i, char j, int k )
//..
// A call to cfi,
//..
//  cfi( doIt )( 33 )( 'a' )( 12 );
//..
// produces the following assembly with g++ -O1.
//..
// 	movl	$12, 8(%esp)
// 	movl	$97, 4(%esp) ; 97 is 'a'
// 	movl	$33, (%esp)
// 	call	__Z4doItici
//..

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/void.hpp>
#include <boost/function.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <sfp/cfunc.hpp>
#include <sfp/detail/misc.hpp>

namespace sfp
{

#ifndef CF_MAX_SIZE
#define CF_MAX_SIZE 12
#endif

#define BOOST_PP_ITERATION_LIMITS (3, CFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/cf_spec.hpp"
#include BOOST_PP_ITERATE()

struct Cf
{
    template<typename T>
    struct result;

    typedef Cf this_type;

#define BOOST_PP_ITERATION_LIMITS (3, CFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/cf_result.hpp"
#include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_LIMITS (3, CFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/cf_func.hpp"
#include BOOST_PP_ITERATE()

};

//..
//  boost::function< boost::function<...(B)> (A)> cf(
//    R (*uncurriedFunc)( A, B,...) )
//..
//
// Return a curried version of the specified 'uncurriedFunc' function pointer
// in the form of a nested 'boost::function' object.
const Cf cf;

struct Cfi
{
    template<typename T>
    struct result;

    typedef Cfi this_type;

#define BOOST_PP_ITERATION_LIMITS (3, CFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/cfi_result.hpp"
#include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_LIMITS (3, CFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/cf_func.hpp"
#include BOOST_PP_ITERATE()

};

//..
//  unspecified cf( R (*uncurriedFunc)( A, B,...) );
//..
//
// Return a curried version of the specified 'uncurriedFunc' function pointer
// in the form of an unspecified functor object. Use of the result object will
// normally result in inline function calls. 
const Cfi cfi;

}

#endif
