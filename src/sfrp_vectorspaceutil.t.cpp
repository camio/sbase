#include <sfrp/vectorspaceutil.t.hpp>

#include <sfrp/vectorspaceutil.hpp>
#include <sfrp/util.hpp>
#include <smisc/point1dvectorspace.hpp>
#include <stest/testcollector.hpp>

namespace sfrp {
void vectorspaceutilTests(stest::TestCollector& col)
{
  col.addTest("sfrp_vectorspaceutil_sum", []()->void {
    Behavior<double> b = sfrp::pmConst(1.0);
    const auto bSum = sfrp::VectorSpaceUtil::sum(b);
    BOOST_CHECK_EQUAL(*bSum.pull(0.0), 1.0);
    BOOST_CHECK_EQUAL(*bSum.pull(1.0), 2.0);
    BOOST_CHECK_EQUAL(*bSum.pull(2.0), 3.0);
  });
}
}
