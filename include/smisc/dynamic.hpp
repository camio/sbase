#ifndef SMISC_DYNAMIC_HPP_
#define SMISC_DYNAMIC_HPP_

#include <boost/any.hpp>
#include <boost/optional.hpp>
#include <boost/function.hpp>
#include <iosfwd>
#include <smisc/doc.hpp>

namespace smisc
{
    struct Dyn
    {
        enum Type
            { Function
            , IO
            , Value
            };

        //Note: don't ever use this constructor as it violates invariants
        Dyn() {}
        Dyn
            ( const boost::any & any_
            , const Type type_
            )
            : any( any_ )
            , type( type_ )
        {
        }
        //Note: if we make this a shared_pointer we will probably get better
        //sharing
        boost::any any;
        Type type;
    };

    typedef boost::optional<Dyn> OpDyn;
    typedef boost::function1<OpDyn, Dyn> DynF;
    typedef boost::function0<Dyn> DynIO;

    namespace detail
    {
        template< typename F >
        struct Generalize0
        {
            Generalize0( F f_ )
                : f( f_ )
            {
            }
            typedef Dyn result_type;
            F f;

            Dyn operator()( ) const
            {
                //See if the argument is of the correct type;
                typedef typename F::result_type Result;
                const Result r = f( );
                return toDyn( r );
            }
        };

        template< typename F >
        DynIO generalize0( F f )
        {
            return Generalize0<F>( f );
        }
        template< typename F >
        struct Generalize
        {
            Generalize( F f_ )
                : f( f_ )
            {
            }
            typedef OpDyn result_type;
            F f;

            OpDyn operator()( const Dyn & argDyn ) const
            {
                //See if the argument is of the correct type;
                typedef typename F::arg1_type Arg;
                typedef typename ::boost::remove_const
                    < typename ::boost::remove_reference<Arg>::type >::type
                    ArgVar;
                ArgVar const * const argPtr = boost::any_cast<ArgVar>( &argDyn.any );
                if( argPtr )
                {
                    typedef typename F::result_type Result;
                    const Arg & arg = *argPtr;
                    const Result r = f( arg );
                    return toDyn( r );
                }
                else
                    return boost::none;
            }
        };

        template< typename F >
        DynF generalize( F f )
        {
            return Generalize<F>( f );
        }
    }

    struct ToDyn
    {
        typedef Dyn result_type;
        template< typename T >
        Dyn operator()( const T & t ) const
        {
            return Dyn( boost::any( t )
                      , Dyn::Value
                      );
        }

        template< typename R >
        Dyn operator()( const boost::function0<R> & f ) const
        {
            return Dyn( boost::any( detail::generalize0( f ) )
                      , Dyn::IO
                      );
        }
        template< typename R >
        Dyn operator()( const boost::function<R ()> & f ) const
        {
            return Dyn( boost::any( detail::generalize0( f ) )
                      , Dyn::IO
                      );
        }
        template< typename R >
        Dyn operator()( R (*f)() ) const
        {
            return Dyn( boost::any
                        ( detail::generalize0
                           ( boost::function<R ()>
                             ( f )))
                      , Dyn::IO
                      );
        }

        template< typename R, typename A1 >
        Dyn operator()( const boost::function1<R,A1> & f ) const
        {
            return Dyn( boost::any( detail::generalize( f ) )
                      , Dyn::Function
                      );
        }
        template< typename R, typename A1 >
        Dyn operator()( const boost::function<R (A1)> & f ) const
        {
            return Dyn( boost::any( detail::generalize( f ) )
                      , Dyn::Function
                      );
        }
        template< typename R, typename A1 >
        Dyn operator()( R (*f)(A1) ) const
        {
            return Dyn( boost::any
                        ( detail::generalize
                           ( boost::function<R (A1)>
                             ( f )))
                      , Dyn::Function
                      );
        }
    } const toDyn = ToDyn();

    inline OpDyn appDyn( const Dyn & a, const Dyn & b )
    {
        switch( a.type )
        {
          case Dyn::Function:
              {
                  DynF const * const fPtr = boost::any_cast<DynF>( &a.any );
                  assert( fPtr );
                  const DynF & f = *fPtr;
                  return f( b );
              }
              break;
          default:
              return boost::none;
              break;
        }
    }
    inline OpDyn runDyn( const Dyn & a )
    {
        switch( a.type )
        {
          case Dyn::IO:
              {
                  DynIO const * const fPtr = boost::any_cast<DynIO>( &a.any );
                  assert( fPtr );
                  const DynIO & f = *fPtr;
                  return boost::make_optional( f() );
              }
              break;
          default:
              return boost::none;
              break;
        }
    }

    Doc dynToDoc( const Dyn & a );

    template< typename T >
    boost::optional<T> fromDyn( const Dyn & a )
    {
        T const * const t = boost::any_cast<T>( &a.any );
        if( t )
            return boost::make_optional( *t );
        else
            return boost::none;
    }
    std::ostream & operator<<( std::ostream & s, const Dyn & a );
}
#endif
