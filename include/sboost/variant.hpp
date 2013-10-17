#ifndef SBOOST_VARIANT_HPP_
#define SBOOST_VARIANT_HPP_

#include <boost/variant.hpp>
#include <boost/version.hpp>
#include <boost/mpl/apply.hpp>

#if _MSC_VER == 1500
#define SBOOST_VARIANT_WORKAROUND
#endif

#if _MSC_VER == 1600 && BOOST_VERSION < 104700
#define SBOOST_VARIANT_WORKAROUND
#endif

#if __GNUC__ == 4 && __GNUC_MINOR__ == 5 && __GNUC_PATCHLEVEL__ == 0 && BOOST_VERSION < 104700
#define SBOOST_VARIANT_WORKAROUND
#endif

#if __GNUC__ == 4 && __GNUC_MINOR__ == 5 && __GNUC_PATCHLEVEL__ == 2 && BOOST_VERSION < 104700
#define SBOOST_VARIANT_WORKAROUND
#endif

#ifdef SBOOST_VARIANT_WORKAROUND
namespace boost
{
    struct recursive_variant_
    {
    };
}
#endif

namespace sboost
{
    namespace mpl = boost::mpl;
    struct variantFix
    {
        template< typename F // this is the functor
                >
        struct apply
            : boost::make_recursive_variant_over
                < typename mpl::apply
                    < F
                    , boost::recursive_variant_
                    >::type
                >
        {
        };
    };
    struct compose
    {
        template< typename F
                , typename G
                >
        struct apply
        {
            struct CType
            {
                template< typename A >
                struct apply
                    : mpl::apply< F
                                , typename mpl::apply< G
                                                     , A
                                                     >::type
                                >
                {
                };
            };
            typedef CType type;
        };
    };
    struct variantFixWith
    {
        template< typename F // this is the functor
                , typename G // this is the extra function over it
                >
        struct apply
            : mpl::apply
                < G
                , typename mpl::apply
                    < variantFix
                    , typename mpl::apply
                       < compose
                       , F
                       , G
                       >::type
                    >::type
                >
        {
        };
    };
}

#endif
