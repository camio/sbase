#include <sfrp/util.hpp>

#include <scpp/misc.hpp>

namespace sfrp
{
    Behavior<bool> pmNot( const Behavior<bool> & p )
    {
        return pmMap( scpp::not_<bool>, p );
    }
    Behavior<bool> pmOr( const Behavior<bool> & a, const Behavior<bool> & b )
    {
        return pmLift( scpp::or_<bool>, a, b );
    }
    Behavior<bool> pmAnd( const Behavior<bool> & a, const Behavior<bool> & b )
    {
        return pmLift( scpp::and_<bool>, a, b );
    }
}
