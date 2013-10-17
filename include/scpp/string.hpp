#ifndef SCPP_STRING_HPP_
#define SCPP_STRING_HPP_

#include <string>
#include <boost/optional.hpp>

namespace scpp
{
    /** Returns the first full line (excluding the end of line characters) in \a
     * buf and leaves \buf without the line or the end of line characters.
     */
    boost::optional<std::string> sPullLine
        ( std::string & buf
        , const std::string & eol
        );
}
#endif
