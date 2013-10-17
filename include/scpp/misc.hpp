#ifndef SCPP_MISC_HPP_
#define SCPP_MISC_HPP_

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace scpp
{
    template< typename T >
    T not_( const T & a )
    {
        return !a;
    }
    template< typename T >
    T and_( const T & a, const T & b )
    {
        return a && b;
    }
    template< typename T >
    T or_( const T & a, const T & b )
    {
        return a || b;
    }
    template< typename T >
    T iff( const bool & a, const T & b, const T & c )
    {
        return a ? b : c;
    }
    template< typename A, typename B >
    auto minus( const A & a, const B & b )
        -> typename boost::remove_reference< typename boost::remove_const< decltype( a - b ) >::type >::type
    {
        return a - b;
    }
    template< typename A, typename B >
    auto plus( const A & a, const B & b )
        -> typename boost::remove_reference< typename boost::remove_const< decltype( a + b ) >::type >::type
    {
        return a + b;
    }
    template< typename A, typename B >
    auto times( const A & a, const B & b )
        -> typename boost::remove_reference< typename boost::remove_const< decltype( a * b ) >::type >::type
    {
        return a * b;
    }
    template< typename A >
    auto negate( const A & a )
        -> typename boost::remove_reference< typename boost::remove_const< decltype( -a ) >::type >::type
    {
        return -a;
    }
}

#endif
