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
#include <sfrp/behaviormap.hpp>
#include <sfrp/behaviorpairutil.hpp>
#include <sfrp/behaviorutil.hpp>
#include <sfrp/eventutil.hpp>
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
    template< typename A >
    Behavior< boost::optional< A > > pmWhen
        ( const Behavior< bool > & b
        , const Behavior< boost::optional<A> > & a
        )
    {
        return sfrp::BehaviorMap()
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
        return sfrp::BehaviorMap()
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
            , sfrp::EventUtil::snapshot( b, e )
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
