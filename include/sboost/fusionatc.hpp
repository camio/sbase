#ifndef SBOOST_FUSIONATC_HPP_
#define SBOOST_FUSIONATC_HPP_

#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/value_at.hpp>

namespace sboost {
    /** This is like the fusion at_c function, but this one can be used in
     * higher order functions once the N argument is known.
     */
    template< int N >
    struct FusionAtC
    {
        typedef FusionAtC<N> this_type;

        template< typename T >
        struct result;

        template< typename S >
        struct result<this_type( const S )>
        {
            typedef typename boost::fusion::result_of::value_at_c<S, N>::type type;
        };

        template< typename S >
        typename result<this_type( const S )>::type
            operator()( const S & s ) const
        {
            return boost::fusion::at_c<N>( s );
        }
    };
}

#endif
