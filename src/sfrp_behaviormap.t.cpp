#include <sfrp/behaviormap.t.hpp>

#include <sfrp/behavior.hpp>
#include <sfrp/behaviormap.hpp>
#include <sfrp/behaviorutil.hpp>
#include <stest/testcollector.hpp>

namespace {
sfrp::Behavior<int> addBehaviors(sfrp::Behavior<int> lhs,
                                 sfrp::Behavior<int> rhs) {
  return sfrp::BehaviorMap()([](int a, int b) { return a + b; }, lhs, rhs);
}
}

namespace sfrp {
void behaviormapTests(stest::TestCollector& col) {
  col.addTest("sfrp_behaviormap_blackBox", []()->void {
    Behavior<std::string> mappedBehavior =
        BehaviorMap()([](int i) { return std::to_string(i); },
                      sfrp::BehaviorUtil::always(3));
    BOOST_CHECK_EQUAL(*mappedBehavior.pull(0.0), "3");
    BOOST_CHECK_EQUAL(*mappedBehavior.pull(2.0), "3");

    Behavior<std::string> mappedBehavior2 =
        BehaviorMap()([](int i,
                         std::string s) { return std::to_string(i) + s; },
                      sfrp::BehaviorUtil::always(3),
                      sfrp::BehaviorUtil::always(std::string("foo")));
    BOOST_CHECK_EQUAL(*mappedBehavior2.pull(0.0), "3foo");
    BOOST_CHECK_EQUAL(*mappedBehavior2.pull(2.0), "3foo");

    Behavior<std::string> mappedBehavior3 =
        BehaviorMap()([](int i, std::string s, double d) {
                        return std::to_string(i) + s;
                      },
                      sfrp::BehaviorUtil::always(3),
                      sfrp::BehaviorUtil::always(std::string("foo")),
                      sfrp::BehaviorUtil::always(4.5));
    BOOST_CHECK_EQUAL(*mappedBehavior3.pull(0.0), "3foo");
    BOOST_CHECK_EQUAL(*mappedBehavior3.pull(2.0), "3foo");
  });
}
}
