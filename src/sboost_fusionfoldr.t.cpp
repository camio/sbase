#include <sboost/fusionfoldr.t.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <sboost/fusionfoldr.hpp>
#include <stest/testcollector.hpp>
#include <string>

namespace {
struct StringConcat {
  typedef std::string result_type;

  template <typename T>
  std::string operator()(const std::string s, T t) const {
    return s + boost::lexical_cast<std::string>(t);
  }
};
}

namespace sboost {
void fusionfoldrTests(stest::TestCollector& col) {
  col.addTest("sboost_fusionfoldr_foldr1", []()->void {
    boost::fusion::vector<std::string, int, unsigned, std::string> v(
        "Hi", 3, 9u, "Ho");
    BOOST_CHECK_EQUAL(sboost::fusion::foldr1(v, StringConcat()), "Hi39Ho");
  });

  col.addTest("sboost_fusionfoldr_foldl1", []()->void {
    boost::fusion::vector<std::string, int, unsigned, std::string> v(
        "Hi", 3, 9u, "Ho");
    BOOST_CHECK_EQUAL(sboost::fusion::foldl1(v, StringConcat()),
                      "Ho93Hi");
  });
}
};
