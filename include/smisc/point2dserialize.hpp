#ifndef SMISC_POINT2D_SERIALIZE_HPP_
#define SMISC_POINT2D_SERIALIZE_HPP_

#include <smisc/point2d.hpp>
#include <boost/serialization/nvp.hpp>

namespace boost { namespace serialization {

    template<class Archive>
    void serialize
        ( Archive & ar
        , smisc::Point2D & p
        , const unsigned int version
        )
    {
        ar & make_nvp( "x", p.x )
           & make_nvp( "y", p.y )
           ;
    }

}}

#endif
