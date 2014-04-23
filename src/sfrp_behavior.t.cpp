#include <sfrp/behavior.t.hpp>

#include <boost/make_shared.hpp>
#include <sfrp/behavior.hpp>
#include <stest/testcollector.hpp>

namespace sfrp {
void behaviorTests(stest::TestCollector& col) {
  col.addTest("sfrp_behavior_undefined", []()->void {
    sfrp::Behavior<int> b;
    BOOST_CHECK(b.pull(0.0)== boost::none);
    BOOST_CHECK(b.pull(1.0)== boost::none);
  });
  col.addTest("sfrp_behavior_splittable", []()->void {
    // Check that two calls of the same time value of copiled behaviors do not
    // both call the underlying function.

    int numCalls = 0;
    sfrp::Behavior<double> b = sfrp::Behavior<double>::fromValuePullFunc(
            [&numCalls](double t)->boost::optional<double> {
              ++numCalls;
              return t;
            });
    sfrp::Behavior<double> c = b;

    BOOST_CHECK(b.pull(0.0) == boost::make_optional(0.0));
    BOOST_CHECK_EQUAL(numCalls, 1 );
    BOOST_CHECK(b.pull(2.0) == boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(numCalls, 2 );
    BOOST_CHECK(b.pull(2.0) == boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(numCalls, 2 );
    BOOST_CHECK(c.pull(2.0) == boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(numCalls, 2 );
    BOOST_CHECK(c.pull(3.0) == boost::make_optional(3.0));
    BOOST_CHECK_EQUAL(numCalls, 3 );
  });
}
}
