#include <sfrp/cachedincreasingpartialtimefunction.t.hpp>

#include <sfrp/cachedincreasingpartialtimefunction.hpp>
#include <stest/testcollector.hpp>

namespace sfrp {
void cachedincreasingpartialtimefunctionTests(stest::TestCollector& col)
{
  col.addTest("sfrp_cachedincreasingpartialtimefunction_pullAtSameTime",
                  []()->void {
    // Check that two calls with the same time argument do not both call the
    // underlying function.
    int numCalls = 0;
    sfrp::CachedIncreasingPartialTimeFunction<double> b(
            [&numCalls](double t)->boost::optional<double> {
              ++numCalls;
              return t;
            });

    BOOST_CHECK(b.pull(0.0) == boost::make_optional(0.0));
    BOOST_CHECK_EQUAL(numCalls, 1);
    BOOST_CHECK(b.pull(2.0) == boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(numCalls, 2);
    BOOST_CHECK(b.pull(2.0) == boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(numCalls, 2);
  });
}
}
