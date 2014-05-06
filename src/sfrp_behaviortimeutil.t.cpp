#include <sfrp/behaviortimeutil.t.hpp>

#include <sfrp/behaviortimeutil.hpp>
#include <stest/testcollector.hpp>

namespace sfrp {
void behaviortimeutilTests(stest::TestCollector& col) {
  col.addTest("sfrp_behaviortimeutil_withPriorPull", []()->void {
    const auto p = sfrp::pmTriggerInfStep(0);
    const auto b = sfrp::BehaviorTimeUtil::withPriorPull(-1, p.first);

    BOOST_CHECK(b.pull(0.0) == std::make_pair(-1, 0));
    BOOST_CHECK(b.pull(1.0) == std::make_pair(0, 0));
    p.second(1);
    BOOST_CHECK(b.pull(2.0) == std::make_pair(0, 1));
    p.second(2);
    BOOST_CHECK(b.pull(3.0) == std::make_pair(1, 2));
  });
}
}
