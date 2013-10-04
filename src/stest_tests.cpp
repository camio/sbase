#include <stest/tests.hpp>

#include <stest/serialization.t.hpp>

namespace stest {
void tests(stest::TestCollector& col) { stest::serializationTests(col); }
}
