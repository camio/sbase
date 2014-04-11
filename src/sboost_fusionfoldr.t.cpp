#include <sboost/fusionfoldr.t.hpp>

#include <boost/fusion/include/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <sboost/fusionfoldr.hpp>
#include <stest/testcollector.hpp>
#include <string>
#include <vector>

namespace {
struct StringConcatRight {
  typedef std::string result_type;
  boost::shared_ptr<std::vector<std::pair<std::string,std::string> >> calls;

  StringConcatRight()
      : calls(boost::make_shared<
            std::vector<std::pair<std::string, std::string>>>()) {}

  template <typename T>
  std::string operator()(T t, const std::string s) const {
    calls->emplace_back( boost::lexical_cast<std::string>(t), s );
    return boost::lexical_cast<std::string>(t) + s;
  }
};

struct StringConcatLeft {
  typedef std::string result_type;
  boost::shared_ptr<std::vector<std::pair<std::string,std::string> >> calls;

  StringConcatLeft()
      : calls(boost::make_shared<
            std::vector<std::pair<std::string, std::string>>>()) {}

  template <typename T>
  std::string operator()(const std::string s, T t) const {
    calls->emplace_back( s, boost::lexical_cast<std::string>(t) );
    return s + boost::lexical_cast<std::string>(t);
  }
};
}

namespace sboost {
void fusionfoldrTests(stest::TestCollector& col) {
  col.addTest("sboost_fusionfoldr_foldr1", []()->void {
    boost::fusion::vector<std::string, int, unsigned, std::string> v(
        "Hi", 3, 9u, "Ho");
    StringConcatRight concat;
    BOOST_CHECK_EQUAL(sboost::foldr1(v, concat), "Hi39Ho");
    std::vector<std::pair<std::string, std::string>> expectedCalls;
    expectedCalls.emplace_back("9", "Ho");
    expectedCalls.emplace_back("3", "9Ho");
    expectedCalls.emplace_back("Hi", "39Ho");
    BOOST_CHECK(*concat.calls == expectedCalls);
  });

  col.addTest("sboost_fusionfoldr_foldl1", []()->void {
    boost::fusion::vector<std::string, int, unsigned, std::string> v(
        "Hi", 3, 9u, "Ho");
    StringConcatLeft concat;
    BOOST_CHECK_EQUAL(sboost::foldl1(v, concat), "Hi39Ho");

    std::vector<std::pair<std::string, std::string>> expectedCalls;
    expectedCalls.emplace_back("Hi", "3");
    expectedCalls.emplace_back("Hi3", "9");
    expectedCalls.emplace_back("Hi39", "Ho");
    BOOST_CHECK(*concat.calls == expectedCalls);
  });
}
};
