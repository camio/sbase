#ifndef SFP_DETAIL_MISC_
#define SFP_DETAIL_MISC_

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace sfp{
    namespace detail{
        template< typename T >
        struct sanitize
            : boost::remove_const< typename boost::remove_reference<T>::type >
        {
        };
    }
}

#endif
