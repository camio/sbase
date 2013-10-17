#ifndef SMISC_ZOI_HPP_
#define SMISC_ZOI_HPP_

namespace smisc
{
    //A value betweeon 0 and 1. Pronounced Zero One Interval
    //μ [[ZOI]] : [0,1]
    struct ZOI
    {
        typedef double Impl;
        explicit ZOI( const Impl impl_ ) : impl( impl_ )
        {
        }
        //invariant: impl ∈ [0,1]
        Impl impl;
    };

    const ZOI zoiZero = ZOI( 0.0 );
    const ZOI zoiOne = ZOI( 1.0 );

    inline double zoiToDouble( const ZOI & z )
    {
        return z.impl;
    }

    inline ZOI clamp01( const double d )
    {
        return ZOI( d < 0 ? 0.0
                  : d > 1 ? 1.0
                  : d
                  );
    }
}

#endif
