#include <sfrp/behaviortimeutil.t.hpp>

#include <sfrp/behaviortimeutil.hpp>
#include <sfrp/triggerutil.hpp>
#include <stest/testcollector.hpp>

static sfrp::Behavior<double> derivative(sfrp::Behavior<double> behavior) {
  return sfrp::BehaviorUtil::map(
      [](std::pair<std::pair<double, double>, std::pair<double, double>>
             pairs) {
        return (pairs.second.first - pairs.first.first) /
               (pairs.second.second - pairs.first.second);
      },
      sfrp::BehaviorTimeUtil::withPriorPull(
          std::make_pair(0.0, 0.0),
          sfrp::BehaviorTimeUtil::withTime(behavior)));
}

namespace sfrp {
void behaviortimeutilTests(stest::TestCollector& col) {
  col.addTest("sfrp_behaviortimeutil_withPriorPull", []()->void {
    const auto p = sfrp::TriggerUtil::triggerInfStep(0);
    const auto b = sfrp::BehaviorTimeUtil::withPriorPull(-1, p.first);

    BOOST_CHECK(b.pull(0.0) == std::make_pair(-1, 0));
    BOOST_CHECK(b.pull(1.0) == std::make_pair(0, 0));
    p.second(1);
    BOOST_CHECK(b.pull(2.0) == std::make_pair(0, 1));
    p.second(2);
    BOOST_CHECK(b.pull(3.0) == std::make_pair(1, 2));
  });
  col.addTest("sfrp_behaviortimeutil_withTime", []()->void {
    sfrp::Behavior<std::pair<int, double>> intBehaviorWithTime =
        sfrp::BehaviorTimeUtil::withTime(sfrp::BehaviorUtil::always(3));

    BOOST_CHECK(intBehaviorWithTime.pull(0.0) == std::make_pair(3, 0.0));
    BOOST_CHECK(intBehaviorWithTime.pull(1.0) == std::make_pair(3, 1.0));
    BOOST_CHECK(intBehaviorWithTime.pull(3.0) == std::make_pair(3, 3.0));
  });
  col.addTest("sfrp_behaviortimeutil_replaceInitialValue", []()->void {
    {
      sfrp::Behavior<int> replacedIntBehavior =
          sfrp::BehaviorTimeUtil::replaceInitialValue(
              sfrp::BehaviorUtil::always(3), 4);

      BOOST_CHECK_EQUAL(replacedIntBehavior.pull(0.0), 4 );
      BOOST_CHECK_EQUAL(replacedIntBehavior.pull(1.0), 3 );
      BOOST_CHECK_EQUAL(replacedIntBehavior.pull(3.0), 3 );
    }
    {
      sfrp::Behavior<int> replacedIntBehavior =
          sfrp::BehaviorTimeUtil::replaceInitialValue(
              sfrp::BehaviorUtil::always(3), 4);

      BOOST_CHECK_EQUAL(replacedIntBehavior.pull(1.0), 4 );
      BOOST_CHECK_EQUAL(replacedIntBehavior.pull(2.0), 3 );
      BOOST_CHECK_EQUAL(replacedIntBehavior.pull(3.0), 3 );
    }
  });
}
}
