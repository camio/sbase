#include <sbasetest/tests.hpp>

#include <smisc/tests.hpp>
#include <stest/tests.hpp>

namespace sbasetest {
void tests(stest::TestCollector& col) {
  smisc::tests(col);
  stest::tests(col);
}
}
