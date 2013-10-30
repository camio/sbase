#include <smisc/classserializerutil.t.hpp>

#include <smisc/classserializerutil.hpp>
#include <stest/testcollector.hpp>

namespace smisc {

void classserializerutilTests(stest::TestCollector& col)
{
  col.addTest("smisc_classserializerutil_unitSerializer", []()->void {
    const smisc::ClassSerializer ser =
        smisc::ClassSerializerUtil::unitSerializer(4);
    BOOST_CHECK_EQUAL(ser.toString(boost::any(4)), "");

    const boost::any result = ser.fromString("");
    BOOST_REQUIRE_NO_THROW(boost::any_cast<int>(result));
    BOOST_CHECK_EQUAL(boost::any_cast<int>(result), 4);
  });
}

}
