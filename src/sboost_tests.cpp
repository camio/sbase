#include <sboost/tests.hpp>

#include <sboost/fusionfoldutil.t.hpp>
#include <sboost/optionalutil.t.hpp>

namespace sboost {
void tests(stest::TestCollector& col) {
  fusionfoldutilTests(col);
  optionalutilTests(col);
}
}

