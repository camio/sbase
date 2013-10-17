#include <smisc/doc.hpp>
#include <boost/bind.hpp>
#include <utf8.h>
#include <ostream>

namespace smisc
{
    typedef std::string::difference_type DT;
    DT dToOutputImpl
        ( std::ostream & os
        , const DT pos
        , const Doc::Impl & d
        );
    struct DToOutput
    {
        typedef DT result_type;
        DT operator()( std::ostream & os
                      , const DT pos
                      , const std::string & s
                      ) const
        {
            os << s;
            return utf8::distance( s.begin(), s.end() ) + pos;
        }
        DT operator()( std::ostream & os
                      , const DT pos
                      , const fus::pair< detail::DRow
                                       , std::pair<Doc::Impl,Doc::Impl>
                                       > & v
                      ) const
        {
            const DT pos2 = dToOutputImpl( os
                                      , pos
                                      , v.second.first
                                      );
            return dToOutputImpl( os
                            , pos2
                            , v.second.second
                            );
        }
        DT operator()( std::ostream & os
                      , const DT pos
                      , const fus::pair< detail::DColumn
                                       , std::pair<Doc::Impl,Doc::Impl>
                                       > & v
                      ) const
        {
            dToOutputImpl( os
                     , pos
                     , v.second.first
                     );
            os << "\n";
            for( DT i = 0; i < pos; ++i )
                os << " ";
            return dToOutputImpl( os
                            , pos
                            , v.second.second
                            );
        }
    };
    DT dToOutputImpl
        ( std::ostream & os
        , const DT pos
        , const Doc::Impl & d
        )
    {
        return boost::apply_visitor
            ( boost::bind( DToOutput()
                         , boost::ref( os )
                         , pos
                         , _1
                         )
            , d
            );
    }
    DT dToOutput
        ( std::ostream & os
        , const DT pos
        , const Doc & d
        )
    {
        return dToOutputImpl( os, pos, d.impl );
    }
    std::ostream & operator<<( std::ostream & os, const smisc::Doc & d )
    {
        dToOutput( os, 0, d );
        return os;
    }
}
