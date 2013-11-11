#include <sfrp/util.t.hpp>

#include <sfrp/util.hpp>
#include <stest/testcollector.hpp>

namespace {
class ReturnType {
} rt;
class Type1 {
} v1;
class Type2 {
} v2;
class Type3 {
} v3;
class Type4 {
} v4;
class Type5 {
} v5;
class Type6 {
} v6;

ReturnType f1(Type1) { return rt; }
ReturnType f2(Type1, const Type2&) { return rt; }
ReturnType f3(Type1, const Type2&, Type3) { return rt; }
ReturnType f4(Type1, const Type2&, Type3, Type4) { return rt; }
ReturnType f5(Type1, const Type2&, Type3, Type4, Type5) { return rt; }
ReturnType f6(Type1, const Type2&, Type3, Type4, Type5, Type6) { return rt; }

void pmLiftCompilationTests() {
  sfrp::Behavior<ReturnType> r = sfrp::pmLift(f1, sfrp::pmConst(v1));
  r = sfrp::pmLift(f2, sfrp::pmConst(v1), sfrp::pmConst(v2));
  r = sfrp::pmLift(f3, sfrp::pmConst(v1), sfrp::pmConst(v2), sfrp::pmConst(v3));
  r = sfrp::pmLift(f4,
                   sfrp::pmConst(v1),
                   sfrp::pmConst(v2),
                   sfrp::pmConst(v3),
                   sfrp::pmConst(v4));
  r = sfrp::pmLift(f5,
                   sfrp::pmConst(v1),
                   sfrp::pmConst(v2),
                   sfrp::pmConst(v3),
                   sfrp::pmConst(v4),
                   sfrp::pmConst(v5));
  r = sfrp::pmLift(f6,
                   sfrp::pmConst(v1),
                   sfrp::pmConst(v2),
                   sfrp::pmConst(v3),
                   sfrp::pmConst(v4),
                   sfrp::pmConst(v5),
                   sfrp::pmConst(v6));
}

ReturnType allRefs( const Type1&, const Type2& )
{
  return rt;
}

void pmEvLiftCompilationTests() {
  sfrp::Behavior<boost::optional<ReturnType>> r =
      sfrp::pmEvLift(f1, sfrp::pmConst(boost::make_optional(v1)));
  r = sfrp::pmEvLift(
      f2, sfrp::pmConst(boost::make_optional(v1)), sfrp::pmConst(v2));
  r = sfrp::pmEvLift(f3,
                     sfrp::pmConst(boost::make_optional(v1)),
                     sfrp::pmConst(v2),
                     sfrp::pmConst(v3));
  r = sfrp::pmEvLift(f4,
                     sfrp::pmConst(boost::make_optional(v1)),
                     sfrp::pmConst(v2),
                     sfrp::pmConst(v3),
                     sfrp::pmConst(v4));
  r = sfrp::pmEvLift(
      allRefs, sfrp::pmConst(boost::make_optional(v1)), sfrp::pmConst(v2));
}
}

namespace sfrp {
void utilTests(stest::TestCollector&col)
{
  col.addTest("sfrp_util_pmWithPrev", []()->void {
    const auto p = sfrp::pmTriggerInfStep(0);
    const auto b = sfrp::pmWithPrev(-1, p.first);

    BOOST_CHECK(b.pull(0.0) == std::make_pair(-1, 0));
    BOOST_CHECK(b.pull(1.0) == std::make_pair(0, 0));
    p.second( 1 );
    BOOST_CHECK(b.pull(2.0) == std::make_pair(0, 1));
    p.second( 2 );
    BOOST_CHECK(b.pull(3.0) == std::make_pair(1, 2));
  });
}
}
