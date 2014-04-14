#include <sfp/tests.hpp>

#include <sfp/cf.t.hpp>
#include <sfp/cfunc.t.hpp>
#include <sfp/ciof.t.hpp>

namespace sfp {
void tests(stest::TestCollector& col) {
  cfTests(col);
  cfuncTests(col);
  ciofTests(col);
}
}
