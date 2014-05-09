#include <sfrp/vectorspaceutil.t.hpp>

#include <sfrp/behavioroperators.hpp>
#include <sfrp/behaviorutil.hpp>
#include <sfrp/vectorspaceutil.hpp>
#include <smisc/point1dvectorspace.hpp>
#include <stest/testcollector.hpp>

static sfrp::Behavior<double> calculatePosition(
    double initialPosition,
    sfrp::Behavior<double> velocity,
    sfrp::Behavior<double> acceleration) {
  return sfrp::BehaviorUtil::always(initialPosition) +
         sfrp::VectorSpaceUtil::integral(
             velocity + sfrp::VectorSpaceUtil::integral(acceleration));
}

namespace sfrp {
void vectorspaceutilTests(stest::TestCollector& col)
{
  col.addTest("sfrp_vectorspaceutil_sum", []()->void {
    Behavior<double> b = sfrp::BehaviorUtil::always(1.0);
    const auto bSum = sfrp::VectorSpaceUtil::sum(b);
    BOOST_CHECK_EQUAL(*bSum.pull(0.0), 1.0);
    BOOST_CHECK_EQUAL(*bSum.pull(1.0), 2.0);
    BOOST_CHECK_EQUAL(*bSum.pull(2.0), 3.0);
  });
  col.addTest("sfrp_vectorspaceutil_integral", []()->void {
    Behavior<double> b = sfrp::BehaviorUtil::pure([](double time) {
      return time <= 1.0 ? 1.0 : 2.0;
    });
    Behavior<double> bIntegrated = sfrp::VectorSpaceUtil::integral(b);
    BOOST_CHECK_EQUAL(bIntegrated.pull(0.0), 0.0);
    BOOST_CHECK_EQUAL(bIntegrated.pull(1.0), 1.0);
    BOOST_CHECK_EQUAL(bIntegrated.pull(2.0), 3.0);
  });
}
}
