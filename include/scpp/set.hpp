#ifndef SCPP_SET_HPP_
#define SCPP_SET_HPP_

#include <set>
#include <algorithm>

namespace scpp
{
    template< typename T >
    std::set<T> setDiff
        ( const std::set< T > & s0
        , const std::set< T > & s1
        )
    {
        std::set<T> r;
        std::set_difference
            ( s0.begin()
            , s0.end()
            , s1.begin()
            , s1.end()
            , std::inserter
                ( r
                , r.begin()
                )
            );
        return r;
    }
    template< typename T >
    bool setContains( const std::set< T > & s0, const T & t )
    {
        return s0.find( t ) != s0.end();
    }
}
#endif
