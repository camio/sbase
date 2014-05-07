#include <sfrp/eventutil.t.hpp>

#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/eventutil.hpp>
#include <stest/testcollector.hpp>

namespace sfrp {
void eventutilTests(stest::TestCollector& col) {
  col.addTest("sfrp_eventutil_never", []()->void {
    sfrp::Behavior<boost::optional<int>> neverInt =
        sfrp::EventUtil::never<int>();
    BOOST_CHECK_EQUAL(neverInt.pull(0.0),
                      boost::make_optional(boost::optional<int>()));
    BOOST_CHECK_EQUAL(neverInt.pull(1.0),
                      boost::make_optional(boost::optional<int>()));
  });
  col.addTest("sfrp_eventutil_mergeWith", []()->void {
    sfrp::Behavior<boost::optional<int>> lhs =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 0.0
                     ? boost::make_optional(boost::make_optional(3))
                     : time == 1.0
                           ? boost::make_optional(boost::make_optional(3))
                           : boost::make_optional(boost::optional<int>());
        });
    sfrp::Behavior<boost::optional<int>> rhs =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 1.0 ? boost::make_optional(boost::make_optional(9))
                             : boost::make_optional(boost::optional<int>());
        });

    sfrp::Behavior<boost::optional<int>> merged =
        sfrp::EventUtil::mergeWith([](int, int) { return 0; }, lhs, rhs);
    BOOST_CHECK_EQUAL(merged.pull(0.0),
                      boost::make_optional(boost::make_optional(3)));
    BOOST_CHECK_EQUAL(merged.pull(1.0),
                      boost::make_optional(boost::make_optional(0)));
    BOOST_CHECK_EQUAL(merged.pull(1.5),
                      boost::make_optional(boost::optional<int>()));
  });
  col.addTest("sfrp_eventutil_filter", []()->void {
    sfrp::Behavior<boost::optional<int>> event =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 0.0 ? boost::make_optional(boost::make_optional(3))
                             : boost::make_optional(boost::make_optional(4));
        });

    sfrp::Behavior<boost::optional<int>> filtered =
        sfrp::EventUtil::filter([](int i) { return i % 2 == 0; }, event);
    // Check that time 0.0 is filtered out because it is not even.
    BOOST_CHECK_EQUAL(filtered.pull(0.0),
                      boost::make_optional(boost::optional<int>()));
    BOOST_CHECK_EQUAL(filtered.pull(1.0),
                      boost::make_optional(boost::make_optional(4)));
  });
  col.addTest("sfrp_eventutil_accumulate", []()->void {

    // Set 'event' to the event that returns '3' at time '0.0' and '4' at time
    // '1.0'.
    sfrp::Behavior<boost::optional<int>> event =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 0.0
                     ? boost::make_optional(boost::make_optional(3))
                     : time == 1.0
                           ? boost::make_optional(boost::make_optional(4))
                           : boost::make_optional(boost::optional<int>());
        });

    // Make summer the same as the above, except it adds its argument
    auto summer = sfrp::EventMap()([](int i) -> boost::function<int(int)> {
                                     return [=](int j) { return i + j; };
                                   },
                                   event);

    //Now accumulate summer
    sfrp::Behavior<boost::optional<int>> accumulated =
        sfrp::EventUtil::accumulate(0, summer);

    // Check that time 0.0 is filtered out because it is not even.
    BOOST_CHECK_EQUAL(accumulated.pull(0.0),
                      boost::make_optional(boost::optional<int>(3)));
    BOOST_CHECK_EQUAL(accumulated.pull(0.5),
                      boost::make_optional(boost::optional<int>()));
    BOOST_CHECK_EQUAL(accumulated.pull(1.0),
                      boost::make_optional(boost::optional<int>(7)));
    BOOST_CHECK_EQUAL(accumulated.pull(1.5),
                      boost::make_optional(boost::optional<int>()));
  });
}
}
