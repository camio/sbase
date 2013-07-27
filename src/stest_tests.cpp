#include <stest/tests.hpp>

#include <stest/serialization.hpp>

namespace stest {
  void tests( stest::TestCollector & col )
  {
    stest::serializationTests( col );
  }
}
