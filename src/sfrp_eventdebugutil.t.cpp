#include <sfrp/eventdebugutil.t.hpp>

#include <sfrp/eventdebugutil.hpp>
#include <stest/testcollector.hpp>

static void example1(sfrp::Behavior<boost::optional<std::string>> stringEvent) {
  sfrp::Behavior<boost::optional<std::string>> watchedEvent =
      sfrp::EventDebugUtil::watch([](std::string s) {
                                    std::cout << "Received " << s << std::endl;
                                  },
                                  stringEvent);
}

namespace sfrp {
void eventdebugutilTests(stest::TestCollector& col) {
  col.addTest("sfrp_eventdebugutil_watch", []()->void {

    sfrp::Behavior<boost::optional<int>> event =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 1.0 || time == 2.0
                     ? boost::make_optional(boost::make_optional(4))
                     : boost::make_optional(boost::optional<int>());
        });

    std::vector<int> retrievedValues;
    Behavior<boost::optional<int>> eventDebugged =
        sfrp::EventDebugUtil::watch([&retrievedValues](int value) {
                                      retrievedValues.push_back(value);
                                    },
                                    event);
    BOOST_CHECK_EQUAL(eventDebugged.pull(0.0),
                      boost::make_optional(boost::optional<int>()));
    BOOST_CHECK_EQUAL(eventDebugged.pull(1.0),
                      boost::make_optional(boost::make_optional(4)));
    BOOST_CHECK_EQUAL(eventDebugged.pull(1.5),
                      boost::make_optional(boost::optional<int>()));
    BOOST_CHECK_EQUAL(eventDebugged.pull(2.0),
                      boost::make_optional(boost::make_optional(4)));

    std::vector<int> expectedRetrievedValues;
    expectedRetrievedValues.push_back(4);
    expectedRetrievedValues.push_back(4);

    BOOST_CHECK(retrievedValues == expectedRetrievedValues);
  });
}
}
