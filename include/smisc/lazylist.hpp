#ifndef SMISC_LAZYLIST_HPP_
#define SMISC_LAZYLIST_HPP_

#include <boost/function.hpp>
#include <boost/utility/result_of.hpp>

/** \file smisc/lazylist.hpp
 *  An implementation of lazy lists using a foreach function as its representative
 */

namespace smisc
{
    template< typename A >
    struct Callback
    {
        typedef boost::function
            < void (const A & )
            > Impl;

        template< typename F >
        Callback( F impl_ )
            : impl( impl_ )
        {
        }

        void operator()( const A & a ) const
        {
            return impl( a );
        }

        Impl impl;
    };

    template< typename A >
    struct LazyList
    {
        typedef A value_type;
        typedef Callback<A> CallbackA;

        typedef boost::function
            < void (const Callback<A> & )
            > ForEach;

        template< typename ForEach_ >
        LazyList( ForEach_ forEach_ )
            : forEach( forEach_ )
        {
        }

        ForEach forEach;
    };

    /** cContraMap : {A B : Set} → (A → B) → Callback B → Callback A
     */
    template< typename A, typename F, typename B >
    auto cContraMap( F f, const Callback<B> & c )
        -> Callback<A>
    {
        return Callback<A>
            ( [=]( const A & a )
              {
                return c.impl( f( a ) );
              }
            );
    }
    /** cFilter : {A : Set} → (A → Bool) → Callback A → Callback A
     */
    template< typename A, typename F >
    Callback<A> cFilter( F f, const Callback<A> & c )
    {
        return Callback<A>
            ( [=]( const A & a )
              {
                if( f( a ) )
                    return c.impl( a );
                else
                    return;
              }
            );
    }

    /** llMap : {A B : Set} → (A → B) → LazyList A → LazyList B
     */
    template< typename F, typename A >
    auto llMap( F f, const LazyList<A> & ll )
        -> LazyList< decltype( f( *(A*)(0) ) ) >
    {
        typedef decltype( f( *(A*)(0) ) ) B;
        return LazyList<B>
            ( [=]( const Callback<B> & b )
              {
                ll.forEach( cContraMap<A>( f, b ) );
              }
            );
    }
    /** llEmpty : (A : Set) → LazyList A
     */
    template< typename A >
    LazyList<A> llEmpty( )
    {
        return LazyList<A>
            ( [=]( const Callback<A> & a )
              {
                return;
              }
            );
    }
    /** llAppend : (A : Set) → LazyList A → LazyList A → LazyList A
     */
    template< typename A >
    LazyList<A> llAppend( const LazyList<A> & a, const LazyList<A> & b )
    {
        return LazyList<A>
            ( [=]( const Callback<A> & c )
              {
                a.forEach( c );
                b.forEach( c );
              }
            );
    }

    /** An infix version of llAppend
     */
    template< typename A >
    LazyList<A> operator+( const LazyList<A> & a, const LazyList<A> & b )
    {
        return llAppend( a, b );
    }

    /** llPure : {A : Set} → A → LazyList A
     */
    template< typename A >
    LazyList<A> llPure( const A & a )
    {
        return LazyList<A>
            ( [=]( const Callback<A> & c )
              {
                c( a );
              }
            );
    }

    /** llFilter : (A : Set) → (A → Bool) → LazyList A → LazyList A
     */
    template< typename F, typename A >
    LazyList< A > llFilter( F f, const LazyList<A> & ll )
    {
        return LazyList<A>
            ( [=]( const Callback<A> & c )
              {
                ll.forEach( cFilter( f, c ) );
              }
            );
    }
}

#endif
