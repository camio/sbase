#include <sfp/cfunc.t.hpp>

#include <boost/type_traits.hpp>
#include <sfp/cfunc.hpp>
#include <type_traits>

// compile-time checks
static_assert(
    std::is_same<sfp::cfunc<char, int, long>::type,
                 boost::function<boost::function<long(int)>(char)>>::value,
    "cfunc unexpected results");

namespace sfp {
void cfuncTests(stest::TestCollector& col) {}
}
