#include <sfrp/behaviorpairutil.t.hpp>

#include <sfrp/behaviorpairutil.hpp>
#include <sfrp/behaviorutil.hpp>
#include <stest/testcollector.hpp>

static void example1() {
  sfrp::Behavior<std::string> stringBehavior;
  sfrp::Behavior<std::pair<std::string, double>> stringBehaviorWithTime =
      sfrp::BehaviorPairUtil::makePair(stringBehavior,
                                       sfrp::BehaviorUtil::time());
}

namespace sfrp {
void behaviorpairutilTests(stest::TestCollector& col) {
  col.addTest("sfrp_behaviorpairutil_makePair", []()->void {
    sfrp::Behavior<std::pair<double, int>> pairBehavior =
        sfrp::BehaviorPairUtil::makePair(sfrp::BehaviorUtil::time(),
                                         sfrp::BehaviorUtil::always(3));
    BOOST_CHECK(pairBehavior.pull(0.0) ==
                boost::make_optional(std::make_pair(0.0, 3)));
    BOOST_CHECK(pairBehavior.pull(1.0) ==
                boost::make_optional(std::make_pair(1.0, 3)));
  });
  col.addTest("sfrp_behaviorpairutil_first", []()->void {
    sfrp::Behavior<double> firstBehavior =
        sfrp::BehaviorPairUtil::first(sfrp::BehaviorPairUtil::makePair(
            sfrp::BehaviorUtil::time(), sfrp::BehaviorUtil::always(3)));
    BOOST_CHECK(firstBehavior.pull(0.0) == boost::make_optional(0.0));
    BOOST_CHECK(firstBehavior.pull(1.0) == boost::make_optional(1.0));
  });
  col.addTest("sfrp_behaviorpairutil_second", []()->void {
    sfrp::Behavior<int> secondBehavior =
        sfrp::BehaviorPairUtil::second(sfrp::BehaviorPairUtil::makePair(
            sfrp::BehaviorUtil::time(), sfrp::BehaviorUtil::always(3)));
    BOOST_CHECK(secondBehavior.pull(0.0) == boost::make_optional(3));
    BOOST_CHECK(secondBehavior.pull(1.0) == boost::make_optional(3));
  });
}
}
