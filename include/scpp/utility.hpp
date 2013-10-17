#ifndef SCPP_UTILITY_HPP_
#define SCPP_UTILITY_HPP_

#include <boost/utility/result_of.hpp>
#include <utility>

namespace scpp
{
    /** Essentially the same as std::make_pair except this isn't an overloaded
     *  function.
     */
    template< typename A, typename B >
    inline std::pair<A, B> make_pair( const A & a, const B & b )
    {
        return std::make_pair( a, b );
    }

    template< typename A, typename B >
    A first
        ( const std::pair< A, B > & pm
        )
    {
        return pm.first;
    }

    template< typename A, typename B >
    B second
        ( const std::pair< A, B > & pm
        )
    {
        return pm.second;
    }

    struct Swap
    {
        typedef Swap this_type;
        template< typename T>
        struct result;

        template< typename A, typename B >
        struct result< this_type( std::pair<A, B> ) >
        {
            typedef std::pair<B,A> type;
        };

        template< typename A, typename B >
        std::pair<A,B> operator()( const std::pair<B,A> & p ) const
        {
            return std::make_pair( p.second, p.first );
        }
    } const swap = Swap();

    namespace detail
    {
        template< typename F >
        struct PFuse
        {
            PFuse( F f_ ) : f( f_ )
            {
            }
            F f;
            typedef PFuse<F> this_type;

            template< typename T >
            struct result;

            template< typename A, typename B >
            struct result< this_type( std::pair<A,B> ) >
            {
                typedef typename boost::result_of<F (A,B)>::type type;
            };

            template< typename A, typename B >
            typename result< this_type( std::pair<A,B> ) >::type
                operator()( const std::pair<A, B> & p ) const
            {
                return f( p.first, p.second );
            }
        };
    }
    /** Converts a two argument function into a one argument function whose
     *  argument is a std::pair.
     */
    template< typename F >
    detail::PFuse<F> pFuse( F f )
    {
        return detail::PFuse<F>( f );
    }

    /** Used as the compairson operator to methods like std::sort. Uses
     * the first (of a pair) as the element of comparison
     */
    struct CompareFirst
    {
        template< typename T >
        bool operator()( const T & a, const T & b ) const
        {
            return a.first < b.first;
        }
    };
}
#endif
