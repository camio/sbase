#include <sfrp/behavioroperators.t.hpp>

#include <sfrp/behavioroperators.hpp>
#include <sfrp/behaviorutil.hpp>
#include <stest/testcollector.hpp>

static void example1()
{
  sfrp::Behavior<double> a;
  sfrp::Behavior<double> b;
  sfrp::Behavior<double> c = a + b;
}

namespace sfrp {
void behavioroperatorsTests(stest::TestCollector& col) {
  col.addTest("sfrp_behavioroperators_minus", []()->void {
    sfrp::Behavior<double> minus =
        sfrp::BehaviorUtil::time() - sfrp::BehaviorUtil::always(1.0);
    BOOST_CHECK_EQUAL(minus.pull(0.0), boost::make_optional(-1.0));
    BOOST_CHECK_EQUAL(minus.pull(1.0), boost::make_optional(0.0));
    BOOST_CHECK_EQUAL(minus.pull(2.0), boost::make_optional(1.0));
  });
  col.addTest("sfrp_behavioroperators_plus", []()->void {
    sfrp::Behavior<double> plus =
        sfrp::BehaviorUtil::time() + sfrp::BehaviorUtil::always(1.0);
    BOOST_CHECK_EQUAL(plus.pull(0.0), boost::make_optional(1.0));
    BOOST_CHECK_EQUAL(plus.pull(1.0), boost::make_optional(2.0));
    BOOST_CHECK_EQUAL(plus.pull(2.0), boost::make_optional(3.0));
  });
  col.addTest("sfrp_behavioroperators_negate", []()->void {
    sfrp::Behavior<double> negate = -sfrp::BehaviorUtil::time();
    BOOST_CHECK_EQUAL(negate.pull(0.0), boost::make_optional(0.0));
    BOOST_CHECK_EQUAL(negate.pull(1.0), boost::make_optional(-1.0));
    BOOST_CHECK_EQUAL(negate.pull(2.0), boost::make_optional(-2.0));
  });
  col.addTest("sfrp_behavioroperators_logicalAnd", []()->void {
    sfrp::Behavior<bool> logicalAnd = sfrp::BehaviorUtil::pure([](double time) {
                                        return time < 1.0 ? false : true;
                                      }) &&
                                      sfrp::BehaviorUtil::pure([](double time) {
                                        return time < 0.5 ? false : true;
                                      });
    BOOST_CHECK(logicalAnd.pull(0.0) == boost::make_optional(false));
    BOOST_CHECK(logicalAnd.pull(0.6) == boost::make_optional(false));
    BOOST_CHECK(logicalAnd.pull(1.6) == boost::make_optional(true));
  });
  col.addTest("sfrp_behavioroperators_logicalOr", []()->void {
    sfrp::Behavior<bool> logicalOr = sfrp::BehaviorUtil::pure([](double time) {
                                        return time < 1.0 ? false : true;
                                      }) ||
                                      sfrp::BehaviorUtil::pure([](double time) {
                                        return time < 0.5 ? false : true;
                                      });
    BOOST_CHECK(logicalOr.pull(0.0) == boost::make_optional(false));
    BOOST_CHECK(logicalOr.pull(0.6) == boost::make_optional(true));
    BOOST_CHECK(logicalOr.pull(1.6) == boost::make_optional(true));
  });
  col.addTest("sfrp_behavioroperators_logicalNot", []()->void {
    sfrp::Behavior<bool> logicalNot = !sfrp::BehaviorUtil::pure([](double time) {
                                        return time < 1.0 ? false : true;
                                      });
    BOOST_CHECK(logicalNot.pull(0.0) == boost::make_optional(true));
    BOOST_CHECK(logicalNot.pull(1.6) == boost::make_optional(false));
  });
  col.addTest("sfrp_behavioroperators_ifThenElse", []()->void {
    sfrp::Behavior<int> ifThenElse = sfrp::BehaviorOperators::ifThenElse(
        sfrp::BehaviorUtil::pure([](double time) {
          return time < 1.0 ? false : true;
        }),
        sfrp::BehaviorUtil::always(3),
        sfrp::BehaviorUtil::always(4));
    BOOST_CHECK(ifThenElse.pull(0.0) == boost::make_optional(4));
    BOOST_CHECK(ifThenElse.pull(1.6) == boost::make_optional(3));
  });
}
}
