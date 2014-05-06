#ifndef SMISC_TVAR_HPP_
#define SMISC_TVAR_HPP_

#include <boost/shared_ptr.hpp>
#include <mutex>

namespace smisc
{
    template< typename T >
    struct TVar
    {
        typedef T value_type;
        TVar( T t_ )
            : tPtr( boost::make_shared<T>( t_ ) )
            , m( boost::make_shared<std::mutex>() )
        {
        }
        //TODO: could this be made into a shared_ptr<std::pair<...>> instead?
        //          -DJS 9/6/2011
        boost::shared_ptr<T> tPtr;
        boost::shared_ptr<std::mutex> m;
    };

    template< typename T >
    T tvGet( const TVar<T> & tv )
    {
        tv.m->lock();
        const T t = *tv.tPtr;
        tv.m->unlock();
        return t;
    }
    template< typename T >
    void tvSet( const TVar<T> & tv, const T & t )
    {
        (*(tv.m)).lock();
        *tv.tPtr = t;
        (*(tv.m)).unlock();
    }
}

#endif
