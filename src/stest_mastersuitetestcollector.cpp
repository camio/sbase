#include <stest/mastersuitetestcollector.hpp>

#include <boost/test/unit_test.hpp>

namespace stest {
void MasterSuiteTestCollector::handleAddTest(
    boost::unit_test::test_case* const tc) {
  boost::unit_test::framework::master_test_suite().add(tc);
}
}
