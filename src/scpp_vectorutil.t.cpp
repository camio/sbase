#include <scpp/vectorutil.t.hpp>

#include <scpp/vectorutil.hpp>
#include <stest/testcollector.hpp>
#include <string>
#include <vector>

static void example1_pushBackIfUnique(std::vector<std::string>& vector,
                                      const std::string s) {
  if (!scpp::VectorUtil::hasElement(vector, s))
    vector.emplace_back(std::move(s));
}

namespace scpp {
void vectorutilTests(stest::TestCollector& col) {
  col.addTest("scpp_vectorutil_hasElement", []()->void {
    std::vector<std::string> vec;
    vec.push_back("Hello");
    vec.push_back("world");
    vec.push_back("stuff");
    BOOST_CHECK(scpp::VectorUtil::hasElement(vec, "Hello"));
    BOOST_CHECK(scpp::VectorUtil::hasElement(vec, "world"));
    BOOST_CHECK(scpp::VectorUtil::hasElement(vec, "stuff"));
    BOOST_CHECK(!scpp::VectorUtil::hasElement(vec, "other"));
    BOOST_CHECK(!scpp::VectorUtil::hasElement(vec, ""));
  });
}
}
