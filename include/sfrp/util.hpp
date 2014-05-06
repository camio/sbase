#ifndef SFRP_UTIL_HPP_
#define SFRP_UTIL_HPP_

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <sboost/optionalutil.hpp>
#include <scpp/operators.hpp>
#include <scpp/pairutil.hpp>
#include <sfp/cf.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/wormhole.hpp>
#include <smisc/unit.hpp>
#include <mutex>
#include <type_traits>

/* There are a few event functions that should be written for completeness's
 * sake. They are...
 *
 *  never : Event a
 *
 *  unionWith : (a -> a -> a) -> Event a -> Event a -> Event a
 *     This one is is a more generalized pmMerge
 *
 *  filterE : (a -> Bool) -> Event a -> Event a
 *     I'm not sure if this one is really necessary since it can be implemented
 *     in terms of something else, like pmEvMap and pmEvJust.
 *
 *  accumE : a -> Event (a -> a) -> Event a
 *     This one is a major missing piece.
 *
 *  I found these out by looking at a reactive-bananna library tutorial slide
 *  from here:
 *  http://apfelmus.nfshost.com/blog/2012/07/15-frp-tutorial-slides.html
 *
 *  - DJS 7/31/2012
 *
 */

namespace sfrp
{
    namespace result_of
    {
        template< typename F_AB
                , typename PM_A
                >
        struct pmMap
        {
            typedef typename PM_A::type A;
            typedef typename std::result_of< F_AB( A ) >::type B;
            typedef Behavior<B> type;
        };
    };
    namespace detail
    {
        template< typename F_AB
                , typename PM_A
                >
        struct PmMap
        {
            typedef typename sfrp::result_of::pmMap<F_AB,PM_A>::A A;
            typedef typename sfrp::result_of::pmMap<F_AB,PM_A>::B B;
            typedef boost::optional<B> result_type;
            PmMap
                ( F_AB f_
                , PM_A pma_
                )
                : f( f_ )
                , pma( pma_ )
            {
            }
            F_AB f;
            PM_A pma;
            result_type operator()( const double time )
            {
                const boost::optional<A> opA = pma.pull( time );
                return opA ? boost::make_optional( f( *opA ) )
                           : boost::none;
            }
        };
    }

    template< typename F_AB
            , typename PM_A
            >
    typename result_of::pmMap< F_AB, PM_A >::type
    pmMap( F_AB f, PM_A pma )
    {
        typedef typename result_of::pmMap< F_AB, PM_A >::type PM_B;
        return PM_B::fromValuePullFunc( detail::PmMap< F_AB, PM_A >( f, pma ) );
    }

    namespace result_of
    {
        template< typename PM_F_AB
                , typename PM_A
                >
        struct pmApp
        {
            typedef typename PM_A::type A;
            typedef typename PM_F_AB::type F_AB;
            typedef typename std::result_of< F_AB( A ) >::type B;
            typedef Behavior<B> type;
        };
    };
    namespace detail
    {
        template< typename PM_F_AB
                , typename PM_A
                >
        struct PmApp
        {
            typedef typename sfrp::result_of::pmApp<PM_F_AB,PM_A>::A A;
            typedef typename sfrp::result_of::pmApp<PM_F_AB,PM_A>::F_AB F_AB;
            typedef typename sfrp::result_of::pmApp<PM_F_AB,PM_A>::B B;

            typedef boost::optional<B> result_type;
            PmApp
                ( PM_F_AB pm0_
                , PM_A pm1_
                )
                : pm0( pm0_ )
                , pm1( pm1_ )
            {
            }
            PM_F_AB pm0;
            PM_A pm1;

            result_type operator()( const double time )
            {
                const boost::optional<F_AB> opF = pm0.pull( time );
                const boost::optional<A> opA = pm1.pull( time );
                return opF && opA
                    ? boost::make_optional( (*opF)(*opA) )
                    : boost::none;
            }
        };
    }
    /** pmApp : Behavior (a → b) → Behavior a → Behavior b
     */
    template< typename PM_F_AB
            , typename PM_A
            >
    typename result_of::pmApp< PM_F_AB, PM_A >::type
    pmApp( const PM_F_AB & pm0, const PM_A & pm1 )
    {
        typedef typename result_of::pmApp< PM_F_AB, PM_A >::type PM_B;
        return PM_B::fromValuePullFunc
            ( detail::PmApp
                < PM_F_AB
                , PM_A
                >
                ( pm0
                , pm1
                )
            );
    }

    template< typename A, typename B >
    Behavior<A> pmFirst
        ( const Behavior< std::pair< A, B > > & pm
        )
    {
        return pmMap( scpp::PairUtil::first<A,B>, pm );
    }
    template< typename A, typename B >
    Behavior<B> pmSecond
        ( const Behavior< std::pair< A, B > > & pm
        )
    {
        return pmMap( scpp::PairUtil::second<A,B>, pm );
    }

    namespace detail
    {
        template< typename T>
        struct PmWatch
        {
            typedef boost::optional<T> result_type;
            PmWatch
                ( const boost::function<void (T) > & f_
                , const Behavior<T> & pm_
                )
                : f( f_ )
                , pm( pm_ )
            {
            }
            result_type operator()( const double time ) const
            {
                result_type r = pm.pull( time );
                if( r )
                    f( *r );
                return r;
            }
            boost::function<void (T) > f;
            Behavior<T> pm;
        };
    }
    /** pmWatch : {T : Set} → IO T → Behavior T → Behavior T
     *  When the input Behavior has never been pulled and never will be pulled,
     *  the resulting Behavior is semantically the same. Everytime a pull is
     *  called, f will be called with its value as a side-effect
     */
    template< typename F, typename T >
    Behavior<T> pmWatch
        ( F f
        , const Behavior<T> & pm
        )
    {
        return Behavior<T>::fromValuePullFunc( detail::PmWatch<T>( f, pm ) );
    }
    namespace detail
    {
        template< typename F, typename T >
        struct PmEvWatch
        {
            typedef void result_type;
            PmEvWatch( F f_ )
                : f( f_ )
            {
            }
            void operator()( const boost::optional<T> & opT )
            {
                if( opT )
                    f( *opT );
            }
            F f;
        };
    }
    /** Same as pmWatch, but for events
     */
    template< typename F, typename T >
    Behavior<boost::optional<T> >
    pmEvWatch
        ( F f
        , const Behavior< boost::optional<T> > & pm
        )
    {
        return pmWatch( detail::PmEvWatch<F,T>(f), pm );
    }
    

    // This is intended to eventually be a series of functions that
    // lift functions of any arity.
    template <typename Func, typename Arg0Beh>
    Behavior<typename std::result_of<Func(typename Arg0Beh::type)>::type>
    pmLift(Func func, Arg0Beh arg0Beh) {
      return pmMap(func, arg0Beh);
    }

    template <typename Func, typename Arg0Beh, typename Arg1Beh>
    Behavior<typename std::result_of<
        Func(typename Arg0Beh::type, typename Arg1Beh::type)>::type>
    pmLift(Func func, Arg0Beh arg0Beh, Arg1Beh arg1Beh) {
      return pmApp(pmMap(sfp::cf(func), arg0Beh), arg1Beh);
    }

    template <typename Func, typename Arg0Beh, typename Arg1Beh, typename Arg2Beh>
    auto pmLift(Func func, Arg0Beh arg0Beh, Arg1Beh arg1Beh, Arg2Beh arg2Beh)
        -> Behavior<decltype(func(*(typename Arg0Beh::type*)(0),
                                  *(typename Arg1Beh::type*)(0),
                                  *(typename Arg2Beh::type*)(0)))> {
      return pmApp(pmApp(pmMap(sfp::cf(func), arg0Beh), arg1Beh), arg2Beh);
    }

    template <typename Func,
              typename Arg0Beh,
              typename Arg1Beh,
              typename Arg2Beh,
              typename Arg3Beh>
    auto pmLift(Func func,
                Arg0Beh arg0Beh,
                Arg1Beh arg1Beh,
                Arg2Beh arg2Beh,
                Arg3Beh arg3Beh)
        -> Behavior<decltype(func(*(typename Arg0Beh::type*)(0),
                                  *(typename Arg1Beh::type*)(0),
                                  *(typename Arg2Beh::type*)(0),
                                  *(typename Arg3Beh::type*)(0)))> {
      return pmApp(
          pmApp(pmApp(pmMap(sfp::cf(func), arg0Beh), arg1Beh), arg2Beh),
          arg3Beh);
    }

    template <typename Func,
              typename Arg0Beh,
              typename Arg1Beh,
              typename Arg2Beh,
              typename Arg3Beh,
              typename Arg4Beh>
    auto pmLift(Func func,
                Arg0Beh arg0Beh,
                Arg1Beh arg1Beh,
                Arg2Beh arg2Beh,
                Arg3Beh arg3Beh,
                Arg4Beh arg4Beh)
        -> Behavior<decltype(func(*(typename Arg0Beh::type*)(0),
                                  *(typename Arg1Beh::type*)(0),
                                  *(typename Arg2Beh::type*)(0),
                                  *(typename Arg3Beh::type*)(0),
                                  *(typename Arg4Beh::type*)(0)))> {
      return pmApp(
          pmApp(pmApp(pmApp(pmMap(sfp::cf(func), arg0Beh), arg1Beh), arg2Beh),
                arg3Beh),
          arg4Beh);
    }

    template <typename Func,
              typename Arg0Beh,
              typename Arg1Beh,
              typename Arg2Beh,
              typename Arg3Beh,
              typename Arg4Beh,
              typename Arg5Beh>
    auto pmLift(Func func,
                Arg0Beh arg0Beh,
                Arg1Beh arg1Beh,
                Arg2Beh arg2Beh,
                Arg3Beh arg3Beh,
                Arg4Beh arg4Beh,
                Arg5Beh arg5Beh)
        -> Behavior<decltype(func(*(typename Arg0Beh::type*)(0),
                                  *(typename Arg1Beh::type*)(0),
                                  *(typename Arg2Beh::type*)(0),
                                  *(typename Arg3Beh::type*)(0),
                                  *(typename Arg4Beh::type*)(0),
                                  *(typename Arg5Beh::type*)(0)))> {
      return pmApp(
          pmApp(pmApp(pmApp(pmApp(pmMap(sfp::cf(func), arg0Beh), arg1Beh),
                            arg2Beh),
                      arg3Beh),
                arg4Beh),
          arg5Beh);
    }
    template <typename Func,
              typename Arg0Beh,
              typename Arg1Beh,
              typename Arg2Beh,
              typename Arg3Beh,
              typename Arg4Beh,
              typename Arg5Beh,
              typename Arg6Beh>
    auto pmLift(Func func,
                Arg0Beh arg0Beh,
                Arg1Beh arg1Beh,
                Arg2Beh arg2Beh,
                Arg3Beh arg3Beh,
                Arg4Beh arg4Beh,
                Arg5Beh arg5Beh,
                Arg6Beh arg6Beh)
        -> Behavior<decltype(func(*(typename Arg0Beh::type*)(0),
                                  *(typename Arg1Beh::type*)(0),
                                  *(typename Arg2Beh::type*)(0),
                                  *(typename Arg3Beh::type*)(0),
                                  *(typename Arg4Beh::type*)(0),
                                  *(typename Arg5Beh::type*)(0),
                                  *(typename Arg6Beh::type*)(0)))> {
      return pmApp(
          pmApp(pmApp(pmApp(pmApp(pmApp(pmMap(sfp::cf(func), arg0Beh), arg1Beh),
                                  arg2Beh),
                            arg3Beh),
                      arg4Beh),
                arg5Beh),
          arg6Beh);
    }


    template< typename A, typename B >
    inline Behavior< std::pair< A, B > > pmZip
        ( const Behavior<A> & a
        , const Behavior<B> & b
        )
    {
        return pmLift
            ( &scpp::PairUtil::makePair< A, B >
            , a
            , b
            );
    }

    /** Returns a pair where the first element is the previous value
     * and the second one is the current value. (prev,cur)
     */
    template< typename T >
    Behavior< std::pair< T, T > >
        pmWithPrev( const T & t0, const Behavior<T> & pm )
    {
        Wormhole<T> w = Wormhole<T>( t0 );
        return pmZip(w.outputBehavior(), w.setInputBehavior(pm));
    }
    namespace detail
    {
        struct PmTime
        {
            typedef boost::optional< double > result_type;
            result_type operator()( const double time ) const
            {
                return time;
            }
        };
    }
    /** Why does this need to be a function instead of a value?
     *  Because of the meaning of Behavior each return of pmTime() has its
     *  own destiny of increasing "Time" arguments
     */
    inline Behavior< double > pmTime( )
    {
        return Behavior< double >::fromValuePullFunc( detail::PmTime( ) );
    }

    template< typename T >
    Behavior< std::pair< T, double > >
        pmWithTime( const Behavior<T> & pm )
    {
        return pmZip( pm, pmTime() );
    }

    namespace detail
    {
        template< typename A >
        A iff( const bool & b, const A & a1, const A & a2 )
        {
            return b ? a1 : a2;
        }
    }
    template< typename BehaviorA >
    BehaviorA pmIff
        ( const Behavior<bool> & b
        , const BehaviorA & a1
        , const BehaviorA & a2
        )
    {
        typedef typename BehaviorA::type A;
        return pmLift( &detail::iff<A>, b, a1, a2 );
    }

    namespace detail
    {
        template< typename A >
        class PmJoin
        {
            typedef sfrp::Behavior
                < A
                > BehaviorA;
            typedef boost::optional
                < std::pair
                    < double //dTime
                    , BehaviorA
                    >
                > State;
            typedef boost::optional
                < BehaviorA
                > OpBehaviorA;
            typedef const sfrp::Behavior
                < OpBehaviorA
                > BehaviorOpBehaviorA;


        public:
            PmJoin
                ( const BehaviorOpBehaviorA & pm_
                )
                : pm( pm_ )
                , state( boost::none )
            {
            }
        public:
            typedef boost::optional< boost::optional< A > > result_type;

            result_type pullVal( const double time )
            {
                const boost::optional<OpBehaviorA> opOpBehaviorA = 
                    pm.pull( time );
                if( opOpBehaviorA )
                {
                    const OpBehaviorA & opBehaviorA = *opOpBehaviorA;
                    if( opBehaviorA )
                    {
                        state = boost::make_optional
                            ( std::make_pair
                                ( time
                                , *opBehaviorA
                                )
                            );
                        const boost::optional<A> opA
                            = state->second.pull( 0 );
                        if( !opA )
                            state = boost::none;
                        return boost::make_optional( opA );
                    }
                    else
                    {
                        if( state )
                        {
                            const boost::optional<A> opA
                                = state->second.pull( time - state->first );
                            if( opA )
                                return boost::make_optional( opA );
                            else
                            {
                                state = boost::none;
                                return result_type( boost::optional<A>());
                            }
                        }
                        else
                        {
                            return result_type( boost::optional<A>());
                        }
                    }
                }
                else
                {
                    if( state )
                    {
                        const boost::optional<A> opA
                            = state->second.pull( time - state->first );
                        if( opA )
                            return boost::make_optional( opA );
                        else
                        {
                            state = boost::none;
                            return boost::none;
                        }
                    }
                    else
                        return boost::none;
                }
            }

            State state;
            BehaviorOpBehaviorA pm;
        };
    }

    /** pmJoin : Behavior (Maybe (Behavior a)) → Behavior (Maybe a)
     * 
     *  Flattens a specially typed and specially valued Behavior.
     *
     * \pre Whenever pm has a Just value, the Just element must be in 0 state.
     * That is, calling pull with a time of 0 should be valid.
     */
    template< typename A >
    sfrp::Behavior
        < boost::optional
          < A
          >
        >
        pmJoin
        ( const sfrp::Behavior
            < boost::optional
               < sfrp::Behavior
                  < A
                  >
               >
            > & pm
        )
    {
        const boost::shared_ptr<detail::PmJoin<A> > pmPtr
            = boost::make_shared<detail::PmJoin<A> >( pm );
        
        return sfrp::Behavior<boost::optional< A > >::fromValuePullFunc
            ( boost::bind
             ( &detail::PmJoin<A>::pullVal
             , pmPtr
             , _1
             )
            );
    }
    namespace detail
    {
        template< typename T >
        struct PmTrigger
        {
            typedef boost::optional<T> OpT;
            typedef boost::optional<OpT> OpOpT;
            typedef boost::optional //is there anything loaded?
                   < boost::optional //Is the thing loaded an "exit" or a
                      < T            // ← value ?
                      >
                   > V;


            boost::optional< boost::optional<T> > pullVal
                ( const double time )
            {
                using boost::make_optional;
                using boost::none;
                using boost::optional;

                m.lock();

                const OpOpT ret
                    = v
                        ? (*v ? OpOpT
                                   ( OpT
                                     (  **v
                                     )
                                   )
                              : OpOpT()
                          )
                        : OpOpT( OpT() )
                        ;
                if( ret )
                    v = OpOpT();
                m.unlock();
                return ret;
            }
            void loadVal( const boost::optional<T> & opT )
            {
                m.lock();
                if( !( v && !(*v ) ) )
                    v = OpOpT( opT );
                m.unlock();
            }
            std::mutex m;
            V v;
        };
    }

    template< typename T >
    struct PmTriggerF
    {
        typedef boost::function
            < void ( const boost::optional< T > & )
            > type;
    };

    /** Creates an event-like behavior that occurs whenever the returned
     * function is called. If the returned function is called with boost::none,
     * it signifies the end of the behavior. The function is thread-safe.
     */
    template< typename T >
    std::pair
        < Behavior< boost::optional< T > >
        , boost::function
            < void ( const boost::optional< T > & )
            >
        >
        pmTrigger()
    {
        const auto fPtr = boost::make_shared<detail::PmTrigger<T> >( );
        return std::make_pair
            ( Behavior<boost::optional< T > >::fromValuePullFunc
             ( boost::bind
              ( &detail::PmTrigger<T>::pullVal
              , fPtr
              , _1
              )
             )
            , boost::bind
              ( &detail::PmTrigger<T>::loadVal
              , fPtr
              , _1
              )
            );
    }
    namespace detail
    {
        template< typename T >
        void pmTriggerInf
            ( const boost::function< void ( const boost::optional<T> & ) > & f
            , const T & t
            )
        {
            f( boost::make_optional( t ) );
        }
    }
    /** Like pmTrigger, but only creates Behaviors that never end.
     */
    template< typename T >
    std::pair
        < Behavior< boost::optional< T > >
        , boost::function
            < void ( const T & )
            >
        >
        pmTriggerInf( )
    {
        auto pairPmF = pmTrigger<T>();
        return std::make_pair
            ( pairPmF.first
            , boost::bind
                ( &detail::pmTriggerInf<T>
                , pairPmF.second
                , _1
                )
            );
    }
    /** Like pmTrigger, but only creates Behaviors that never end. Also, the
     *  returned Behavior is a step function with \a t0 as its initial value.
     */
    template< typename T >
    std::pair
        < Behavior< T >
        , boost::function
            < void ( const T & )
            >
        >
        pmTriggerInfStep( const T & t0 )
    {
        auto pairPmF = pmTriggerInf<T>();
        return std::make_pair
            ( pmStep
                ( t0
                , pairPmF.first
                )
            , pairPmF.second
            );
    }
    
    namespace detail
    {
        template< typename T >
        T pmStepHelper( const boost::optional<T> & cur, const T & prev )
        {
            return cur
                ? *cur
                : prev
                ;
        }
    }
    /** Conceptually turns an event into a behavior. Whatever the value
     *  of the event \a pm is, the behavior will have that value until the
     *  next occurance.
     */
    template<typename T>
    Behavior<T> pmStep
        ( const T & t0
        , const Behavior< boost::optional<T> > & pm
        )
    {
        auto w = Wormhole<T>( t0 );
        return w.setInputBehavior(
            pmLift(&detail::pmStepHelper<T>, pm, w.outputBehavior()));
    }
    
    template< typename A>
    Behavior< boost::optional<A> > 
    pmMerge
        ( const Behavior< boost::optional<A> > & a
        , const Behavior< boost::optional<A> > & b
        )
    {
        return pmLift( sboost::OptionalUtil::alternative<A>, a, b );
    }
    namespace detail
    {
        template
            < typename A
            , typename B
            >
        boost::optional
            < std::pair
              < A
              , B
              >
            > snapshotHelper
                ( const B & b
                , const boost::optional<A> & opA
                )
        {
            return opA
                ? boost::make_optional
                    ( std::make_pair
                     ( *opA
                     , b
                     )
                    )
                : boost::none
                ;
        }

    }

    template
        < typename A
        , typename B
        >
    Behavior
        < boost::optional
          < std::pair
            < A
            , B
            >
          >
        >
    pmSnapshot
        ( const Behavior< B > & b
        , const Behavior< boost::optional< A > > & e
        )
    {
        return pmLift( detail::snapshotHelper<A,B>, b, e );
    }
    namespace detail
    {
        template
            < typename A
            , typename B
            >
        boost::optional
            < B
            > snapshot_Helper
                ( const B & b
                , const boost::optional<A> & opA
                )
        {
            return opA
                ? boost::make_optional ( b )
                : boost::none
                ;
        }

    }

    //pmSnapshot_ b e = map snd (pmSnapshot b e)
    // Once we get some functor instances for optional, the simple
    // implementation will be doable. Otherwise, :(.
    template
        < typename A
        , typename B
        >
    Behavior
        < boost::optional
          < B
          >
        >
    pmSnapshot_
        ( const Behavior< B > & b
        , const Behavior< boost::optional< A > > & e
        )
    {
        return pmLift( detail::snapshot_Helper<A,B>, b, e );
    }

    namespace detail
    {
        template< typename A >
        struct PmDelay
        {
            PmDelay
                ( const A & a_
                , const Behavior<A> & pm_
                )
                : prev( boost::make_shared<boost::optional<A> >( a_ ) )
                , pm( pm_ )
            {
            }
            boost::optional<A> operator()
                ( const double time
                ) const
            {
                if( *prev )
                {
                    const A r = **prev;
                    *prev = pm.pull( time );
                    return boost::make_optional( r );
                }
                else
                    return boost::none;
            }
            boost::shared_ptr<boost::optional<A> > prev;
            Behavior<A> pm;
        };
    }

    /** returns a copy of \a pm such that pull with time t gives
     * the pull of pm at the prior pull time. the first pull of the result
     * returns \a a.
     *
     * Note: it is a beginners mistake to overuse this function. Be sure that
     * its use is really the best option.
     */
    template< typename A >
    Behavior<A> pmDelay( const A & a, const Behavior<A> & pm )
    {
        return Behavior<A>::fromValuePullFunc( detail::PmDelay<A>( a, pm ) );
    }

    Behavior<bool> pmNot( const Behavior<bool> & p );
    Behavior<bool> pmOr( const Behavior<bool> &, const Behavior<bool> & );
    Behavior<bool> pmAnd( const Behavior<bool> &, const Behavior<bool> & );

    inline Behavior<bool> operator!( const Behavior<bool> & p )
    {
        return pmNot( p );
    }

    inline Behavior<bool> operator||( const Behavior<bool> & a, const Behavior<bool> & b )
    {
        return pmOr( a, b );
    }
    inline Behavior<bool> operator&&( const Behavior<bool> & a, const Behavior<bool> & b )
    {
        return pmAnd( a, b );
    }
    template< typename A, typename B >
    inline auto operator-( const Behavior<A> & a, const Behavior<B> & b )
        -> decltype( pmLift( scpp::Operators::minus<A,B>, a, b ) )
    {
        return pmLift( scpp::Operators::minus<A,B>, a, b );
    }
    template< typename A, typename B >
    inline auto operator+( const Behavior<A> & a, const Behavior<B> & b )
        -> decltype( pmLift( scpp::Operators::plus<A,B>, a, b ) )
    {
        return pmLift( scpp::Operators::plus<A,B>, a, b );
    }

    template< typename A >
    inline auto operator-( const Behavior<A> & a )
        -> decltype( pmApp( scpp::Operators::negate<A>, a ) )
    {
        return pmApp( scpp::Operators::negate<A>, a );
    }

    template< typename A >
    Behavior< boost::optional< A > > pmWhen
        ( const Behavior< bool > & b
        , const Behavior< boost::optional<A> > & a
        )
    {
        return pmLift
            ( scpp::Operators::iff< boost::optional<A> >
            , b
            , a
            , pmConst( boost::optional<A>() )
            );
    }
    namespace result_of
    {
        template< typename F, typename Ev >
        struct pmEvMap
        {
            typedef typename Ev::type OpT;
            typedef typename OpT::value_type T;
            typedef typename sboost::OptionalUtil_MapResult<F,T>::type OpU;
            typedef Behavior< OpU > type;
        };
    }
    /** Like pmMap, but particularly for Events
     */
    template< typename F, typename Ev >
    typename result_of::pmEvMap<F, Ev>::type
    pmEvMap( F f, const Ev & ev )
    {
        typedef typename Ev::type OpA;
        typedef typename OpA::value_type A;
        return pmMap
            ( sfp::cf( &sboost::OptionalUtil::map<F,A> )( f )
            , ev
            );
    }

    /** Converts an Event (Op a) into an Event a, removing all
     *  none instances.
     */
    template< typename A >
    Behavior< boost::optional< A > >
    pmEvJust
        ( const Behavior
           < boost::optional
             < boost::optional
               < A
               >
             >
           > & pm
        )
    {
        return pmMap( sboost::OptionalUtil_Join(), pm );
    }

    template< typename A >
    Behavior< A > pmCurtail
        ( const Behavior< smisc::Unit > & slice
        , const Behavior< A > & a
        )
    {
        return pmSecond( pmZip( slice, a ) );
    }
    namespace result_of
    {
        template
            < typename F
            , typename A
            >
        struct pmEvApp
        {
            typedef typename std::result_of<F (A)>::type B;
            typedef Behavior< boost::optional<B> > type;

        };
    }
    namespace detail
    {
        template
            < typename F
            , typename A
            >
        struct PmEvApp
        {
            typedef typename std::result_of<F (A)>::type B;
            typedef B result_type;
            B operator()
                ( const std::pair<F,A> p
                ) const
            {
                return p.first( p.second );
            }
        };
    }
    template
        < typename F
        , typename A
        >
    typename result_of::pmEvApp<F,A>::type
    pmEvApp( const Behavior< boost::optional< F > > & e
           , const Behavior< A > & b
           )
    {
        return pmEvMap
            ( detail::PmEvApp<F,A>()
            , pmSnapshot( b, e )
            );
    }

    template <typename Func, typename Arg0Beh>
    auto pmEvLift(Func func, Arg0Beh arg0Beh)
        -> Behavior<
              boost::optional<decltype(func(*(typename typename Arg0Beh::type::value_type*)(0)))>> {
      return pmEvMap(func, arg0Beh);
    }

    template <typename Func, typename Arg0Beh, typename Arg1Beh>
    auto pmEvLift(Func func, Arg0Beh arg0Beh, Arg1Beh arg1Beh)
        -> Behavior<
              boost::optional<decltype(func(*(typename typename Arg0Beh::type::value_type*)(0),
                                            *(typename Arg1Beh::type*)(0)))>> {
      return pmEvApp(pmEvMap(sfp::cf(func), arg0Beh), arg1Beh);
    }

    template <typename Func,
              typename Arg0Beh,
              typename Arg1Beh,
              typename Arg2Beh>
    auto pmEvLift(Func func, Arg0Beh arg0Beh, Arg1Beh arg1Beh, Arg2Beh arg2Beh)
        -> Behavior<
              boost::optional<decltype(func(*(typename typename Arg0Beh::type::value_type*)(0),
                                            *(typename Arg1Beh::type*)(0),
                                            *(typename Arg2Beh::type*)(0)))>> {
      return pmEvApp(pmEvApp(pmEvMap(sfp::cf(func), arg0Beh), arg1Beh),
                     arg2Beh);
    }

    template <typename Func,
              typename Arg0Beh,
              typename Arg1Beh,
              typename Arg2Beh,
              typename Arg3Beh>
    auto pmEvLift(Func func,
                  Arg0Beh arg0Beh,
                  Arg1Beh arg1Beh,
                  Arg2Beh arg2Beh,
                  Arg3Beh arg3Beh)
        -> Behavior<
              boost::optional<decltype(func(*(typename typename Arg0Beh::type::value_type*)(0),
                                            *(typename Arg1Beh::type*)(0),
                                            *(typename Arg2Beh::type*)(0),
                                            *(typename Arg3Beh::type*)(0)))>> {
      return pmEvApp(
          pmEvApp(pmEvApp(pmEvMap(sfp::cf(func), arg0Beh), arg1Beh), arg2Beh),
          arg3Beh);
    }
}
#endif
