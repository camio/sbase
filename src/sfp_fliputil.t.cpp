#include <sfp/fliputil.t.hpp>

#include <boost/function.hpp>
#include <sfp/fliputil.hpp>
#include <stest/testcollector.hpp>
#include <string>

namespace {
class A{};
class B{};

int foo(A a, B b) { return 0; }
int bar(boost::function<int(B, A)> f) { return 0; }

void f() { bar(sfp::FlipUtil::flip(foo)); }
}
static int testFunc( int i, std::string s ) {
  return i;
}

namespace sfp {
void fliputilTests(stest::TestCollector& col) {
  col.addTest("sfp_fliputil_flipFunctor", []()->void {
    BOOST_CHECK_EQUAL(FlipUtil_Flip()([](int, std::string s) { return s; })("foo", 3),
                      "foo");
  });
  col.addTest("sfp_fliputil_flipFunction", []()->void {
    BOOST_CHECK_EQUAL(FlipUtil::flip([](int, std::string s) { return s; })("foo", 3),
                      "foo");
  });
}
}
