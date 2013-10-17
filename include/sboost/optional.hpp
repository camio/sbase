#ifndef SBOOST_OPTIONAL_HPP_
#define SBOOST_OPTIONAL_HPP_

#include <boost/optional.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/result_of.hpp>

namespace sboost
{
    struct OpJoin
    {
        typedef OpJoin this_type;

        template< typename A >
        struct result;

        template< typename A >
        struct result
            < this_type (boost::optional< boost::optional< A > >)
            >
        {
            typedef boost::optional<A> type;
        };
        
        template< typename A >
        boost::optional<A> operator()
            ( const boost::optional< boost::optional< A > > & opOpA
            )
        {
            return opOpA ? *opOpA
                         : boost::none
                         ;
        }

    } const opJoin;

    template< typename A >
    boost::optional<A> opAlt
        ( const boost::optional<A> & a
        , const boost::optional<A> & b
        )
    {
        return a ? a : b;
    }

    struct OpHasValue
    {
        typedef bool result_type;
        template< typename T >
        bool operator()( const boost::optional<T> & o ) const
        {
            return o;
        }
    } const opHasValue;

    template< typename T >
    T opFlatten( const T & t, const boost::optional<T> & opT )
    {
        return opT ? *opT : t;
    }

    namespace result_of
    {
        template< typename F, typename A >
        struct opMap
        {
            typedef typename boost::result_of<F(A)>::type B;
            //We don't handle optional references and, since references
            //are out, remove any const qualifier.
            typedef typename boost::remove_reference
                < typename boost::remove_const
                    < B
                    >::type
                >::type BClean;
            typedef boost::optional<BClean> type;
        };
    }
    template< typename F, typename A >
    auto opMap( F f, const boost::optional<A> & op ) -> decltype( boost::make_optional( f( *op ) ) )
    {
        return op ? boost::make_optional( f( *op ) )
                  : boost::none
                  ;
    }

    template< typename F, typename A >
    typename boost::result_of<F(A)>::type
    opBind( const boost::optional<A> & a, F f )
    {
        return a ? f( *a ) : boost::none;
    }
}

template< typename A >
boost::optional<A> operator-( const boost::optional<A> & a, const A & b )
{
    return a ? boost::make_optional( *a - b )
             : boost::none;
}

#endif
