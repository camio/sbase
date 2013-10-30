#include <smisc/classserializerboostserializableutil.t.hpp>

#include <boost/optional.hpp>
#include <boost/serialization/optional.hpp>
#include <smisc/classserializerboostserializableutil.hpp>
#include <stest/testcollector.hpp>

namespace smisc {
void classserializerboostserializableutilTests(stest::TestCollector& col)
{
  col.addTest("smisc_classserializerboostserializableutil_fromSerializable",
                  []()->void {
    const smisc::ClassSerializer ser =
        ClassSerializerBoostSerializableUtil::classSerializerFromSerializable<
            boost::optional<int>>();
    const std::string s = ser.toString(boost::any(boost::make_optional(3)));
    boost::any result = ser.fromString(s);
    BOOST_REQUIRE_NO_THROW(boost::any_cast<boost::optional<int>>(result));
    BOOST_CHECK(boost::any_cast<boost::optional<int>>(result) ==
                boost::make_optional(3));
  });
}
}
