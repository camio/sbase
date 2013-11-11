#include <sbasetest/tests.hpp>

#include <sfp/tests.hpp>
#include <smisc/tests.hpp>
#include <stest/tests.hpp>

namespace sbasetest {
void tests(stest::TestCollector& col) {
  sfp::tests(col);
  smisc::tests(col);
  stest::tests(col);
}
}
