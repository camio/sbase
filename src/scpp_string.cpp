#include <scpp/string.hpp>
#include <algorithm>
#include <boost/utility.hpp>

namespace scpp
{
    boost::optional<std::string> sPullLine
        ( std::string & buf
        , const std::string & eol
        )
    {
        const auto i = std::search
            ( buf.begin()
            , buf.end()
            , eol.begin()
            , eol.end()
            );
        if( i == buf.end() )
            return boost::none;
        else
        {
            const std::string r = std::string( buf.begin(), i );
            buf = std::string( boost::next( i, 2 ), buf.end() );
            return r;
        }
    }
}
