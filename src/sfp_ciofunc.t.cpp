#include <sfp/ciofunc.t.hpp>

#include <sfp/ciofunc.hpp>
#include <type_traits>

// compile-time checks
static_assert(
    std::is_same<sfp::ciofunc<char, int, long>::type,
                 boost::function<boost::function<boost::function<long()>(int)>(
                     char)>>::value,
    "ciofunc unexpected results");

namespace sfp {
void ciofuncTests(stest::TestCollector& col) {}
}
