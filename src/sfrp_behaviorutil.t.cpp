#include <sfrp/behaviorutil.t.hpp>

#include <boost/optional/optional_io.hpp>
#include <smisc/unit.hpp>
#include <sfrp/behaviorutil.hpp>
#include <stest/testcollector.hpp>
#include <cmath>  // std::atan2, std::sin

namespace {

static void example1() {
  sfrp::Behavior<int> always3 = sfrp::BehaviorUtil::always(3);
  sfrp::Behavior<double> time = sfrp::BehaviorUtil::time();

  sfrp::Behavior<double> timeTimesTwo =
      sfrp::BehaviorUtil::pure([](double time) { return time * 2; });

  sfrp::Behavior<std::string> switchAtTime3 = sfrp::BehaviorUtil::pure([](
      double time) { return time < 3 ? std::string("hello") : "world"; });
}

struct Drawing {};

static void example2() {
  sfrp::Behavior<Drawing> loopingAnimation;
  sfrp::Behavior<smisc::Unit> undefinedAfter500 =
      sfrp::Behavior<smisc::Unit>::fromValuePullFunc([](double time) {
        return time < 500.0 ? boost::make_optional(smisc::Unit()) : boost::none;
      });
  sfrp::Behavior<Drawing> loopingAnimationUntil500 =
      sfrp::BehaviorUtil::curtail(loopingAnimation, undefinedAfter500);
}
static sfrp::Behavior<double> example3_sin(sfrp::Behavior<double> value) {
  return sfrp::BehaviorUtil::map([](double d) { return std::sin(d); }, value);
}

static sfrp::Behavior<double> example3_atan2(sfrp::Behavior<double> y,
                                             sfrp::Behavior<double> x) {
  return sfrp::BehaviorUtil::map([](double y,
                                    double x) { return std::atan2(y, x); },
                                 y,
                                 x);
}
}

namespace sfrp {
void behaviorutilTests(stest::TestCollector& col) {
  col.addTest("sfrp_behaviorutil_always", []()->void {
    sfrp::Behavior<int> always3 = sfrp::BehaviorUtil::always(3);
    BOOST_CHECK_EQUAL(always3.pull(0.0), boost::make_optional(3));
    BOOST_CHECK_EQUAL(always3.pull(1.0), boost::make_optional(3));
    BOOST_CHECK_EQUAL(always3.pull(2.0), boost::make_optional(3));
  });
  col.addTest("sfrp_behaviorutil_time", []()->void {
    sfrp::Behavior<double> time = sfrp::BehaviorUtil::time();
    BOOST_CHECK_EQUAL(time.pull(0.0), boost::make_optional(0.0));
    BOOST_CHECK_EQUAL(time.pull(1.0), boost::make_optional(1.0));
    BOOST_CHECK_EQUAL(time.pull(2.0), boost::make_optional(2.0));
  });
  col.addTest("sfrp_behaviorutil_pure", []()->void {
    sfrp::Behavior<double> pure =
        sfrp::BehaviorUtil::pure([](double time) { return time * 2.0; });
    BOOST_CHECK_EQUAL(pure.pull(0.0), boost::make_optional(0.0));
    BOOST_CHECK_EQUAL(pure.pull(1.0), boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(pure.pull(2.0), boost::make_optional(4.0));
  });
  col.addTest("sfrp_behaviorutil_curtail", []()->void {
    sfrp::Behavior<int> curtailed = sfrp::BehaviorUtil::curtail(
        sfrp::BehaviorUtil::always(3),
        sfrp::Behavior<std::string>::fromValuePullFunc([](double time) {
          return time < 2.0 ? boost::make_optional(std::string(""))
                            : boost::none;
        }));
    BOOST_CHECK_EQUAL(curtailed.pull(0.0), boost::make_optional(3));
    BOOST_CHECK_EQUAL(curtailed.pull(1.0), boost::make_optional(3));
    BOOST_CHECK_EQUAL(curtailed.pull(2.1), boost::none);
  });
  col.addTest("sfrp_behaviorutil_map", []()->void {
    sfrp::Behavior<double> mapped =
        sfrp::BehaviorUtil::map([](double a, double b) { return a + b; },
                                sfrp::BehaviorUtil::always(1.0),
                                sfrp::BehaviorUtil::time());
    BOOST_CHECK_EQUAL(mapped.pull(0.0), boost::make_optional(1.0));
    BOOST_CHECK_EQUAL(mapped.pull(1.0), boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(mapped.pull(3.0), boost::make_optional(4.0));
  });
}
}
