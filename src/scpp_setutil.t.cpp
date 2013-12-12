#include <scpp/setutil.t.hpp>

#include <scpp/setutil.hpp>
#include <stest/testcollector.hpp>
#include <set>
#include <string>

namespace scpp {
void setutilTests(stest::TestCollector& col) {
  col.addTest("scpp_setutil_difference", []()->void {
    std::set<std::string> set;
    set.insert("foo");
    set.insert("bar");
    set.insert("baz");

    std::set<std::string> subtractor;
    subtractor.insert("foo");
    subtractor.insert("bar");
    subtractor.insert("joe");

    std::set<std::string> expectedDifference;
    expectedDifference.insert("baz");

    BOOST_CHECK(expectedDifference ==
                scpp::SetUtil::difference(set, subtractor));
  });
  col.addTest("scpp_setutil_contains", []()->void {
    std::set<std::string> set;
    set.insert("foo");
    set.insert("bar");
    set.insert("baz");

    BOOST_CHECK(scpp::SetUtil::contains(set, "foo"));
    BOOST_CHECK(scpp::SetUtil::contains(set, "bar"));
    BOOST_CHECK(!scpp::SetUtil::contains(set, "joe"));
  });
}
}
