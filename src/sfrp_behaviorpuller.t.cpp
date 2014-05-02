#include <sfrp/behaviorpuller.t.hpp>

#include <sfrp/behavior.hpp>
#include <sfrp/behaviorpuller.hpp>
#include <stest/testcollector.hpp>

static void example() {
  // Set 'intTime' to the behavior that is the integer value of whatever time
  // it is.
  sfrp::Behavior<int> intTime = sfrp::Behavior<int>::fromValuePullFunc([](
      double time) { return int(time); });

  // Set 'alwaysFour' to a behavior that is "Four" for all times.
  sfrp::Behavior<std::string> alwaysFour =
      sfrp::Behavior<std::string>::fromValuePullFunc([](double time) {
        return std::string("Four");
      });

  sfrp::BehaviorPuller atOne(1.0);
  // Will be 'make_optional(1)'
  boost::optional<int> intTimeReult = atOne( intTime );
  // Will be 'make_optional(string("Four"))'
  boost::optional<std::string> alwaysFourResult = atOne( alwaysFour );
}

namespace sfrp {
void behaviorpullerTests(stest::TestCollector& col) {
  col.addTest("sfrp_behaviorpuller_misc", []()->void {

    sfrp::Behavior<int> intTime = sfrp::Behavior<int>::fromValuePullFunc([](
        double time) { return int(time); });

    sfrp::Behavior<int> doubleIntTime =
        sfrp::Behavior<int>::fromValuePullFunc([](double time) {
          return int(time) * 2;
        });

    sfrp::BehaviorPuller atZero(0.0);
    BOOST_CHECK(atZero(intTime) == boost::make_optional(0));
    BOOST_CHECK(atZero(intTime) == boost::make_optional(0));
    BOOST_CHECK(atZero(doubleIntTime) == boost::make_optional(0));

    sfrp::BehaviorPuller atOne(1.0);
    BOOST_CHECK(atOne(intTime) == boost::make_optional(1));
    BOOST_CHECK(atOne(intTime) == boost::make_optional(1));
    BOOST_CHECK(atOne(doubleIntTime) == boost::make_optional(2));
  });
}
}
