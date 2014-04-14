#ifndef SMISC_DOC_HPP_
#define SMISC_DOC_HPP_

#include <iosfwd>
#include <string>
#include <vector>
#include <boost/assign/list_of.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>

/** \file smisc/doc.hpp
 *  A simple pretty printing library.
 *
 *  To use, create a Doc which are created by combining horizontally
 *  type-set docs, vertically type-set docs, and dStr's. When output, it
 *  arrainges the pieces maintaining these spacial constraints. Use operator<<
 *  or boost::lexical_cast to get the output.
 */

namespace smisc
{
    using boost::assign::list_of;
    namespace fus = boost::fusion;

    namespace detail
    {
        struct DRow{};
        struct DColumn{};
    }

    struct Doc
    {
        typedef boost::make_recursive_variant
            < fus::pair< detail::DRow
                       , std::pair< boost::recursive_variant_
                                  , boost::recursive_variant_
                                  >
                       >
            , fus::pair< detail::DColumn
                       , std::pair< boost::recursive_variant_
                                  , boost::recursive_variant_
                                  >
                       >
            , std::string
            >::type Impl;
        explicit Doc( const Impl & impl_ )
            : impl( impl_ )
        {
        }
        Impl impl;
    };

    inline Doc dHor( const Doc & d1, const Doc & d2 )
    {
        return Doc
            ( fus::make_pair<detail::DRow>
             ( std::make_pair
              ( d1.impl
              , d2.impl ) ) );
    }
    inline Doc dVert( const Doc & d1, const Doc & d2 )
    {
        return Doc
            ( fus::make_pair<detail::DColumn>
             ( std::make_pair
              ( d1.impl
              , d2.impl ) ) );
    }
    inline Doc dStr( const std::string & s )
    {
        return Doc( s );
    }
    inline Doc dInt( const int i )
    {
        return Doc( boost::lexical_cast<std::string>(i) );
    }
    inline Doc dDouble( const double i )
    {
        return Doc( boost::lexical_cast<std::string>(i) );
    }
    inline Doc dBool( const bool b )
    {
        return Doc( std::string( b ? "True"
                                   : "False"
                               )
                  );
    }
    //extra functions
    template< typename Iter >
    Doc dHorL( Iter start, Iter end )
    {
        if( start == end )
            return dStr("");
        else if( boost::next( start ) == end )
            return *start;
        else
            return dHor( *start
                       , dHorL( boost::next( start )
                              , end
                              )
                       );
    }
    template< typename Iter >
    Doc dVertL( Iter start, Iter end )
    {
        if( start == end )
            return dStr("");
        else if( boost::next( start ) == end )
            return *start;
        else
            return dVert( *start
                       , dVertL( boost::next( start )
                              , end
                              )
                       );
    }
    inline Doc dHorL( const std::vector< Doc > & ds )
    {
        return dHorL( ds.begin(), ds.end() );
    }
    inline Doc dVertL( const std::vector< Doc > & ds )
    {
        return dVertL( ds.begin(), ds.end() );
    }

    inline Doc dParen( const Doc & d )
    {
        return dHorL( list_of( dStr( "(" )
                            )( d
                            )( dStr( ")" )
                             )
                    );
    }
    /** Assuming \a pos is the horizontal position of the cursor, outputs
     *  \a d and returns the end horizontal position of the cursor.
     */
    std::string::difference_type dToOutput
        ( std::ostream & os
        , const std::string::difference_type pos
        , const Doc & d
        );

    std::ostream & operator<<( std::ostream & os, const Doc & d );
}

#endif
