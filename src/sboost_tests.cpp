#include <sboost/tests.hpp>

#include <sboost/fusionfoldutil.t.hpp>
#include <sboost/optionalutil.t.hpp>
#include <sboost/serializablestringutil.t.hpp>

namespace sboost {
void tests(stest::TestCollector& col) {
  fusionfoldutilTests(col);
  optionalutilTests(col);
  serializablestringutilTests(col);
}
}

