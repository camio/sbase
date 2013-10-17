#include <smisc/lazylist.hpp>
#include <string>

namespace smisc
{

    template< typename F >
    void forEach( F f )
    {
        f( 12 );
        f( 13 );
    }

    void f()
    {
        LazyList<int> ll
            = LazyList<int>
                ( []( const Callback<int> & c )
                    { c( 12 ); c( 14 ); }
                );
        LazyList<std::string> ll2
            = llMap
                ( []( const int ){ return std::string("Hi"); }
                , ll
                );
    }
};
