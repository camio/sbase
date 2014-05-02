#include <sfp/tests.hpp>

#include <sfp/cf.t.hpp>
#include <sfp/cfunc.t.hpp>
#include <sfp/ciof.t.hpp>
#include <sfp/ciofunc.t.hpp>
#include <sfp/flipped.t.hpp>
#include <sfp/fliputil.t.hpp>

namespace sfp {
void tests(stest::TestCollector& col) {
  cfTests(col);
  cfuncTests(col);
  ciofTests(col);
  ciofuncTests(col);
  flippedTests(col);
  fliputilTests(col);
}
}
