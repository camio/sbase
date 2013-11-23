#include <sfrp/util.hpp>

#include <scpp/operators.hpp>

namespace sfrp
{
    Behavior<bool> pmNot( const Behavior<bool> & p )
    {
        return pmMap( scpp::Operators::logicalNot<bool>, p );
    }
    Behavior<bool> pmOr( const Behavior<bool> & a, const Behavior<bool> & b )
    {
        return pmLift( scpp::Operators::logicalOr<bool>, a, b );
    }
    Behavior<bool> pmAnd( const Behavior<bool> & a, const Behavior<bool> & b )
    {
        return pmLift( scpp::Operators::logicalAnd<bool>, a, b );
    }
}
