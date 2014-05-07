#include <sfrp/behaviordebugutil.t.hpp>

#include <boost/optional/optional_io.hpp>
#include <sfrp/behaviordebugutil.hpp>
#include <sfrp/behaviorutil.hpp>
#include <stest/testcollector.hpp>
#include <iostream>
#include <string>
#include <vector>

static void example1(sfrp::Behavior<std::string> behaviorString) {
  sfrp::Behavior<std::string> watchedString =
      sfrp::BehaviorDebugUtil::watch([](std::string s) {
                                       std::cout << "Received " << s
                                                 << std::endl;
                                     },
                                     behaviorString);
}

namespace sfrp {
void behaviordebugutilTests(stest::TestCollector& col) {
  col.addTest("sfrp_behaviordebugutil_watch", []()->void {
    sfrp::Behavior<double> time = sfrp::BehaviorUtil::time();
    std::vector<double> retrievedValues;
    Behavior<double> timeDebugged =
        sfrp::BehaviorDebugUtil::watch([&retrievedValues](double time) {
                                         retrievedValues.push_back(time);
                                       },
                                       time);
    BOOST_CHECK_EQUAL(timeDebugged.pull(0.0), boost::make_optional(0.0));
    BOOST_CHECK_EQUAL(timeDebugged.pull(1.0), boost::make_optional(1.0));
    BOOST_CHECK_EQUAL(timeDebugged.pull(2.0), boost::make_optional(2.0));

    std::vector<double> expectedRetrievedValues;
    expectedRetrievedValues.push_back(0.0);
    expectedRetrievedValues.push_back(1.0);
    expectedRetrievedValues.push_back(2.0);

    BOOST_CHECK(retrievedValues == expectedRetrievedValues);
  });
}
}
