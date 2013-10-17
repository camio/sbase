#ifndef TCHAN_HPP_
#define TCHAN_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <list>

/** \file smisc/TChan.hpp
 * A channel used to communicate between threads
 */

namespace smisc
{
    template< typename T >
    struct TChan
    {
        typedef T value_type;
        TChan( )
            : tPtr( boost::make_shared<std::list<T> >( ) )
            , m( boost::make_shared<boost::mutex>() )
        {
        }
        boost::shared_ptr<std::list<T> > tPtr;
        boost::shared_ptr<boost::mutex> m;
    };
    template< typename TChanT, typename T >
    void tcWrite( const TChanT & tc, const T & t )
    {
        tc.m->lock();
        tc.tPtr->push_back( t );
        tc.m->unlock();
    }
    template< typename T >
    bool tcEmpty( const TChan<T> & tc )
    {
        tc.m->lock();
        const bool b = tc.tPtr->empty();
        tc.m->unlock();
        return b;
    }
    template< typename T >
    boost::optional<T> tcRead( const TChan<T> & tc )
    {
        boost::optional<T> r;
        tc.m->lock();
        if( !tc.tPtr->empty() )
        {
            r = tc.tPtr->front();
            tc.tPtr->pop_front();
        }
        tc.m->unlock();
        return r;
    }
}
#endif
