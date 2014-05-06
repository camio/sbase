#include <sfrp/eventmapfunctionadapter.t.hpp>

#include <boost/optional/optional_io.hpp>
#include <sfrp/eventmapfunctionadapter.hpp>
#include <stest/testcollector.hpp>

namespace sfrp {
void eventmapfunctionadapterTests(stest::TestCollector& col) {
  col.addTest("sfrp_eventmapfunctionadapter_misc", []()->void {
    auto f = [](int i, int j) { return i + j; };
    typedef decltype(f) F;
    EventMapFunctionAdapter<F> adapter(f);
    BOOST_CHECK_EQUAL(adapter(boost::make_optional(3), 4),
                      boost::make_optional(7));
    BOOST_CHECK_EQUAL(adapter(boost::optional<int>(), 4), boost::none);
  });
}
}
