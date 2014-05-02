#include <sboost/serializablestringutil.t.hpp>

#include <sboost/serializablestringutil.hpp>
#include <stest/testcollector.hpp>

template< typename T >
static T clone( const T & t ) {
  return *sboost::SerializableStringUtil::fromString<T>(
      sboost::SerializableStringUtil::toString(t));
}

namespace sboost {
void serializablestringutilTests(stest::TestCollector& col)
{
  col.addTest("sboost_serializablestringutil_misc", []()->void {
    BOOST_CHECK_EQUAL(SerializableStringUtil::fromString<int>(
                          SerializableStringUtil::toString(3)),
                      3);
    BOOST_CHECK_EQUAL(clone(3), 3);
  });
}
}
