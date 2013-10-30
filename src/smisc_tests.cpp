#include <smisc/tests.hpp>

#include <smisc/anyserializer.t.hpp>
#include <smisc/classserializerutil.t.hpp>
#include <smisc/classserializerboostserializableutil.t.hpp>

namespace smisc {
void tests(stest::TestCollector& col) {
  smisc::anyserializerTests(col);
  smisc::classserializerboostserializableutilTests(col);
  smisc::classserializerutilTests(col);
}
}
