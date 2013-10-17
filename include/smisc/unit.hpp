#ifndef SMISC_UNIT_HPP_
#define SMISC_UNIT_HPP_

namespace smisc
{
    struct Unit
    {
        bool operator==( const Unit & ) const { return true; }
    } const unit = Unit();
}

#endif
