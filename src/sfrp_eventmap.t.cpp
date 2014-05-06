#include <sfrp/eventmap.t.hpp>

#include <boost/optional/optional_io.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/behaviorutil.hpp>
#include <sfrp/eventmap.hpp>
#include <stest/testcollector.hpp>

namespace sfrp {
void eventmapTests(stest::TestCollector& col) {
  col.addTest("sfrp_eventmap_misc", []()->void {
    sfrp::Behavior<boost::optional<int>> lhs =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 1.0 || time == 2.0
                     ? boost::make_optional(boost::make_optional(3))
                     : boost::make_optional(boost::optional<int>());
        });
    sfrp::Behavior<int> rhs = sfrp::BehaviorUtil::always(5);

    sfrp::Behavior<boost::optional<int>> result =
        sfrp::EventMap()([](int i, int j) { return i + j; }, lhs, rhs);

    BOOST_CHECK_EQUAL(result.pull(1.0),
                      boost::make_optional(boost::make_optional(8)));
    BOOST_CHECK_EQUAL(result.pull(1.5),
                      boost::make_optional(boost::optional<int>()));
    BOOST_CHECK_EQUAL(result.pull(2.0),
                      boost::make_optional(boost::make_optional(8)));
    BOOST_CHECK_EQUAL(result.pull(2.1),
                      boost::make_optional(boost::optional<int>()));
  });
}
}
