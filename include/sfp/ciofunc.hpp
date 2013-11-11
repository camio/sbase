#ifndef SFP_CIOFUNC_HPP_
#define SFP_CIOFUNC_HPP_

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

#ifndef CIOFUNC_MAX_SIZE
#define CIOFUNC_MAX_SIZE 10
#endif

#define CIOFUNC_PRINT(z, n, data) data

#define CIOFUNC_CR(z, n, data) BOOST_PP_CAT(data, n) = boost::mpl::void_

template< BOOST_PP_ENUM( BOOST_PP_ADD( CIOFUNC_MAX_SIZE, 1), CIOFUNC_CR, class T) > 
struct ciofunc;

//base case
template< typename A >
struct ciofunc< A
            BOOST_PP_COMMA_IF( BOOST_PP_GREATER( CIOFUNC_MAX_SIZE, 1 ) )
            BOOST_PP_ENUM( BOOST_PP_SUB( CIOFUNC_MAX_SIZE, 1 )
                         , CIOFUNC_PRINT
                         , boost::mpl::void_
                         )
            >
{
    typedef boost::function< A () > type;
};


#define BOOST_PP_ITERATION_LIMITS (2, CIOFUNC_MAX_SIZE )
#define BOOST_PP_FILENAME_1 "sfp/detail/ciofunc_spec.hpp"
#include BOOST_PP_ITERATE()

#undef CIOFUNC_CR
#undef CIOFUNC_PRINT

}
#endif
