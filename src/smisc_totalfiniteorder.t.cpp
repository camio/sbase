#include <smisc/totalfiniteorder.t.hpp>

#include <boost/assign/list_of.hpp>
#include <smisc/totalfiniteorder.hpp>
#include <stest/testcollector.hpp>

namespace smisc {

void totalfiniteorderTests(stest::TestCollector& col) {
  col.addTest("smisc_totalfiniteorder_sorted",
                  []()->void {
    const std::vector<std::string> v =
        boost::assign::list_of(std::string("a"))("b")("c");
    const TotalFiniteOrder tfo =
        *tfoFromVector(boost::assign::list_of(size_t(1))(size_t(2))(size_t(0)));
    const std::vector<std::string> expectedResult =
        boost::assign::list_of(std::string("b"))("c")("a");
    const std::vector<std::string> actualResult = tfoSorted(tfo, v);
    BOOST_CHECK_EQUAL_COLLECTIONS(expectedResult.begin(),
                                  expectedResult.end(),
                                  actualResult.begin(),
                                  actualResult.end());
  });
}
}
