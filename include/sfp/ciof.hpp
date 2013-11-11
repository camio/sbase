#ifndef SFP_CIOF_HPP_
#define SFP_CIOF_HPP_

//@PURPOSE: Provide routines to curry IO functions.
//
//@FUNCTIONS:
//  sfp::ciof: function to curried IO boost::function converter
//  sfp::ciofi: function to curried IO functor converter
//
//@SEE_ALSO: sfp_cf
//
//@DESCRIPTION: This component provides a means to convert function pointers
// into curried variants where the the innermost function has no arguments.
//
// Given a function 'f' with 'i' arguments,
//..
//  R f( A₁, A₂, …, Aᵢ );
//..
//
// , 'ciofi(f)' or 'ciof(f)' is an IO curried form of 'f'.
//
// A value of type 'R' can be computed with 'cfi(f)(a₁)(a₂)⋯(aᵢ)()' where
// 'aᵣ:Aᵣ'.
//
// The extra inner function 'ciof' returns, as opposed to 'cf', is useful in
// pure functional programming concepts to distinguish "impure" functions that
// perform some kind of IO.
//
// Differences between 'cf' and 'cfi'
// ----------------------------------
// 
// The main difference between 'ciof' (curried function) and 'cfioi' (curried
// function inline) is that the latter doesn't return a boost function and
// therefore incurs no allocation and calls are inline.
//
// Usage
// -----
// This section illustrates intended use of this component.
//


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
#include <sfp/ciofunc.hpp>
#include <sfp/detail/misc.hpp>

namespace sfp
{

#ifndef CIOF_MAX_SIZE
#define CIOF_MAX_SIZE 12
#endif

#define BOOST_PP_ITERATION_LIMITS (1, CIOFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/ciof_spec.hpp"
#include BOOST_PP_ITERATE()

struct Ciof
{
    template<typename T>
    struct result;

    typedef Ciof this_type;

#define BOOST_PP_ITERATION_LIMITS (1, CIOFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/ciof_result.hpp"
#include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_LIMITS (1, CIOFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/ciof_func.hpp"
#include BOOST_PP_ITERATE()

} const ciof = Ciof();

struct Ciofi
{
    template<typename T>
    struct result;

    typedef Ciofi this_type;

#define BOOST_PP_ITERATION_LIMITS (1, CIOFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/ciofi_result.hpp"
#include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_LIMITS (1, CIOFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/ciof_func.hpp"
#include BOOST_PP_ITERATE()

} const ciofi = Ciofi();

}

#endif
