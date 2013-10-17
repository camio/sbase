#include <smisc/dynamic.hpp>
#include <smisc/unit.hpp>
#include <ostream>

namespace smisc
{
    Doc dynToDoc( const Dyn & a )
    {
        if( a.type == Dyn::Function )
            return dStr( "<func>" );
        else if( a.any.type() == typeid(int) )
            return dInt( boost::any_cast<int>( a.any ) );
        else if( a.any.type() == typeid(double) )
            return dDouble( boost::any_cast<double>( a.any ) );
        else if( a.any.type() == typeid(bool) )
            return dBool(boost::any_cast<bool>( a.any ));
        else if( a.any.type() == typeid(std::string) )
            return dStr(boost::any_cast<std::string>( a.any ));
        else if( a.any.type() == typeid(Unit) )
            return dStr( "⊤" );
        else
            return dStr( "CppInternal" );
    }
    std::ostream & operator<<( std::ostream & s, const Dyn & a )
    {
        return (s << dynToDoc( a ) );
    }
}
