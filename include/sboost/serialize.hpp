#ifndef SBOOST_SERIALIZE_HPP_
#define SBOOST_SERIALIZE_HPP_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/optional.hpp>
#include <sstream>

namespace sboost
{
    template< typename T >
    std::string serializableToString( const T & a )
    {
        std::ostringstream oss;
        boost::archive::text_oarchive oa( oss );
        oa << a;
        return oss.str();
    }
    template< typename T >
    boost::optional<T> serializableFromString( const std::string & a )
    {
        std::istringstream iss( a );
        boost::archive::text_iarchive ia( iss );
        T t;
        try
        {
            ia >> t;
            return boost::make_optional( t );
        }
        catch( boost::archive::archive_exception & )
        {
            return boost::none;
        }
    }
}
#endif
