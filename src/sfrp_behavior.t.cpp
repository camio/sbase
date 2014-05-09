#include <sfrp/behavior.t.hpp>

#include <boost/make_shared.hpp>
#include <sfrp/behavior.hpp>
#include <stest/testcollector.hpp>
#include <chrono>
#include <ctime>
#include <list>
#include <thread>
#include <utility>
#include <iterator>  // std::next

namespace {
sfrp::Behavior<int> example1() {
  sfrp::Behavior<int> alwaysThree = sfrp::Behavior<int>::fromValuePullFunc([](
      double time) { return boost::make_optional(3); });
  return alwaysThree;
}

template <typename Value>
sfrp::Behavior<Value> always(const Value& value) {
  return sfrp::Behavior<Value>::fromValuePullFunc([value](double time) {
    return boost::make_optional(value);
  });
}
void example1Rev() {
  sfrp::Behavior<int> alwaysThree = always( 3 );
}

sfrp::Behavior<int> operator+( sfrp::Behavior<int> lhs, sfrp::Behavior<int> rhs ) {
  return sfrp::Behavior<int>::fromValuePullFunc([lhs, rhs](double time)
                                                    ->boost::optional<int> {
    boost::optional<int> lhsVal = lhs.pull(time);
    boost::optional<int> rhsVal = rhs.pull(time);
    if (lhsVal && rhsVal)
      return boost::make_optional(*lhsVal + *rhsVal);
    else
      return boost::none;
  });
}

std::pair<int, int> getMousePos();

std::pair<int, int> getMousePos() { return std::make_pair(0, 0); }

void example3()
{
  sfrp::Behavior<std::pair<int, int>> mousePos =
      sfrp::Behavior<std::pair<int, int>>::fromValuePullFunc([](double time) {
        return boost::make_optional(getMousePos());
      });
}

double currentTime();
double currentTime() { return 0.0; }

void example3Revised()
{
  boost::shared_ptr<
      std::list<sfrp::CachedPull<std::pair<int, int>>>> mousePosQueue =
      boost::make_shared<std::list<sfrp::CachedPull<std::pair<int, int>>>>();
  // Intialize the queue
  mousePosQueue->emplace_front(
      sfrp::CachedPull<std::pair<int, int>>(currentTime(), getMousePos()));

  // Asynchronous Execution 1
  {
    mousePosQueue->emplace_front(
        sfrp::CachedPull<std::pair<int, int>>(currentTime(), getMousePos()));
  }

  sfrp::Behavior<std::pair<int, int>> mousePos =
      sfrp::Behavior<std::pair<int, int>>::fromValuePullFunc([mousePosQueue](
          double time) {
        // Look in our queue for the first entry with a time less than 'time'.
        auto iterator =
            std::find_if(mousePosQueue->begin(),
                         mousePosQueue->end(),
                         [time](sfrp::CachedPull<std::pair<int, int>> val) {
              return val.time() <= time;
            });

        // We've requested a time before our initial value. This indicates a
        // programming error.
        assert(iterator != mousePosQueue->end());

        // Clear the cache we don't need. We can do this because we know all
        // future calls will have an argument greater than 'time'.
        if (std::next(iterator) != mousePosQueue->end())
          mousePosQueue->erase(std::next(iterator), mousePosQueue->end());

        return boost::make_optional(iterator->value());
      });
}

struct Drawing {
};
void draw(Drawing) {}

void example4( ) {
  sfrp::Behavior<Drawing> drawingBehavior;

  const std::time_t initialCalendarTime = std::time(0);
  std::time_t currentCalendarTime = initialCalendarTime;
  for (;;) {
    const double time = std::difftime( currentCalendarTime, initialCalendarTime );
    boost::optional<Drawing> drawing = drawingBehavior.pull(time);
    if( drawing )
      draw( *drawing );
    else
      // Behavior no longer defined
      return;
    currentCalendarTime = std::time(0);
  }
}
void example4_2( ) {
  sfrp::Behavior<Drawing> drawingBehavior;

  const std::time_t initialCalendarTime = std::time(0);
  std::time_t currentCalendarTime = initialCalendarTime;
  for (;;) {
    const double time = std::difftime( currentCalendarTime, initialCalendarTime );
    boost::optional<Drawing> drawing = drawingBehavior.pull(time);
    if( drawing )
      draw( *drawing );
    else
      // Behavior no longer defined
      return;

    currentCalendarTime = std::time(0);
    const double nextTime = std::difftime( currentCalendarTime, initialCalendarTime );
    const double minimumSecondsBetweenFrames = 1.0/60.0;
    if( minimumSecondsBetweenFrames > nextTime - time ) {
      std::this_thread::sleep_for(std::chrono::milliseconds(
          int(1000 * (minimumSecondsBetweenFrames - (nextTime - time)))));
      currentCalendarTime = std::time(0);
    }
  }
}

}

namespace sfrp {
void behaviorTests(stest::TestCollector& col) {
  col.addTest("sfrp_behavior_undefined", []()->void {
    sfrp::Behavior<int> b;
    BOOST_CHECK(b.pull(0.0) == boost::none);
    BOOST_CHECK(b.pull(1.0) == boost::none);
  });
  col.addTest("sfrp_behavior_splittable", []()->void {
    // Check that two calls of the same time value of copiled behaviors do not
    // both call the underlying function.

    int numCalls = 0;
    sfrp::Behavior<double> b = sfrp::Behavior<double>::fromValuePullFunc(
            [&numCalls](double t)->boost::optional<double> {
              ++numCalls;
              return t;
            });
    sfrp::Behavior<double> c = b;

    BOOST_CHECK(b.pull(0.0) == boost::make_optional(0.0));
    BOOST_CHECK_EQUAL(numCalls, 1);
    BOOST_CHECK(b.pull(2.0) == boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(numCalls, 2);
    BOOST_CHECK(b.pull(2.0) == boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(numCalls, 2);
    BOOST_CHECK(c.pull(2.0) == boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(numCalls, 2);
    BOOST_CHECK(c.pull(3.0) == boost::make_optional(3.0));
    BOOST_CHECK_EQUAL(numCalls, 3);
  });
  col.addTest("sfrp_behavior_example1", []()->void {
    sfrp::Behavior<int> always3 = example1();
    BOOST_CHECK_EQUAL(always3.pull(0.0), 3);
    BOOST_CHECK_EQUAL(always3.pull(1.0), 3);
    BOOST_CHECK_EQUAL(always3.pull(1000.0), 3);
    sfrp::Behavior<int> always4 = always(4);
    BOOST_CHECK_EQUAL(always4.pull(0.0), 4);
    BOOST_CHECK_EQUAL(always4.pull(1.0), 4);
    BOOST_CHECK_EQUAL(always4.pull(1000.0), 4);
  });
  col.addTest("sfrp_behavior_example2", []()->void {
    sfrp::Behavior<int> always3 = example1();
    sfrp::Behavior<int> always4 = always(4);
    sfrp::Behavior<int> sum = always3 + always4;
    BOOST_CHECK_EQUAL(sum.pull(0.0), 7);
    BOOST_CHECK_EQUAL(sum.pull(1.0), 7);
    BOOST_CHECK_EQUAL(sum.pull(1000.0), 7);
  });
}
}
