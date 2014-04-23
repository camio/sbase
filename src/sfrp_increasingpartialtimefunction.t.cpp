#include <sfrp/increasingpartialtimefunction.t.hpp>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <sfrp/increasingpartialtimefunction.hpp>
#include <stest/testcollector.hpp>
#include <iostream>

// The behavior is undefined unless the specified 'time' is greater than the
// last call to this function. The behavior is also undefined unless the
// previous call to 'timeFunction()' did not return 'boost::none'.
static boost::optional<int> timeFunction(double time) { return 3; }

// The behavior is undefined unless the specified 'time' is greater than the
// last call to this function.
static boost::optional<int> relaxedTimeFunction(double time) {
  static sfrp::IncreasingPartialTimeFunction<int> wrappedTimeFunction(
      timeFunction);
  return wrappedTimeFunction.pull(time);
}

namespace sfrp {
void increasingpartialtimefunctionTests(stest::TestCollector& col) {
  col.addTest("sfrp_increasingpartialtimefunction_callPastNone", []()->void {
    // Check that the 'pull()' function never calls the underlying function
    // after none is returned.
    bool noneReturned = false;
    bool calledPastNone = false;
    sfrp::IncreasingPartialTimeFunction<double>
        b([&noneReturned, &calledPastNone](double t)->boost::optional<double> {
          if (t < 1.0)
            return t;
          else {
            if (noneReturned)
              calledPastNone = true;
            else
              noneReturned = true;
            return boost::none;
          }
        });
    BOOST_CHECK(b.pull(0.0) == boost::make_optional(0.0));
    BOOST_CHECK(b.pull(0.5) == boost::make_optional(0.5));
    BOOST_CHECK_EQUAL(noneReturned, false);
    BOOST_CHECK(b.pull(1.5) == boost::none);
    BOOST_CHECK_EQUAL(noneReturned, true);
    BOOST_CHECK(b.pull(2.0) == boost::none);
    BOOST_CHECK_EQUAL(noneReturned, true);
    BOOST_CHECK_EQUAL(calledPastNone, false);
  });
  col.addTest("sfrp_increasingpartialtimefunction_garbageCollection",
                  []()->void {
    // Check that a 'pull()' that returns 'boost::none' will have the
    // side-effect of garbage collecting the function supplied to
    // 'fromValuePullFunc()'.

    boost::shared_ptr<int> intPtr = boost::make_shared<int>(3);
    sfrp::IncreasingPartialTimeFunction<double> b(
            [intPtr](double t)->boost::optional<double> {
              if (t < 1.0)
                return t;
              else {
                return boost::none;
              }
            });
    BOOST_CHECK_EQUAL(intPtr.use_count(), 2);
    BOOST_CHECK(b.pull(0.0) == boost::make_optional(0.0));
    BOOST_CHECK(b.pull(0.5) == boost::make_optional(0.5));
    BOOST_CHECK_EQUAL(intPtr.use_count(), 2);
    BOOST_CHECK(b.pull(1.5) == boost::none);
    BOOST_CHECK_EQUAL(intPtr.use_count(), 1);
  });
}
}
