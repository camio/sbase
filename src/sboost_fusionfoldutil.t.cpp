#include <sboost/fusionfoldutil.t.hpp>

#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sboost/fusionfoldutil.hpp>
#include <stest/testcollector.hpp>
#include <algorithm>  // std::min
#include <string>
#include <vector>

namespace {

struct Min {
  template <typename T>
  struct result;

  template <typename T, typename U>
  struct result<Min(T, U)> {
    typedef typename boost::remove_const<
        typename boost::remove_reference<T>::type>::type TValue;
    typedef typename boost::remove_const<
        typename boost::remove_reference<U>::type>::type UValue;

    typedef typename boost::mpl::if_<
        typename boost::mpl::or_<boost::is_same<TValue, float>,
                                 boost::is_same<UValue, float>>::type,
        float,
        int>::type type;
  };

  int operator()(int i, int j) const { return std::min(i, j); }
  float operator()(int i, float j) const { return std::min(float(i), j); }
  float operator()(float i, float j) const { return std::min(i, j); }
  float operator()(float i, int j) const { return std::min(i, float(j)); }
};

template <typename Sequence>
auto minimum(Sequence s) -> decltype(sboost::FusionFoldUtil::foldl1(s, Min())) {
  return sboost::FusionFoldUtil::foldl1(s, Min());
}

void testExample()
{
  const auto vector0 = boost::fusion::make_vector( 3, 22.5f, 0 );
  const auto vector1 = boost::fusion::make_vector( 3, 6, 0 );

  float vector0Min = minimum( vector0 );
  int vector1Min = minimum( vector1 );
}
}

namespace {
struct StringConcatRight {
  typedef std::string result_type;
  boost::shared_ptr<std::vector<std::pair<std::string, std::string>>> calls;

  StringConcatRight()
      : calls(boost::make_shared<
            std::vector<std::pair<std::string, std::string>>>()) {}

  template <typename T>
  std::string operator()(T t, const std::string s) const {
    calls->emplace_back(boost::lexical_cast<std::string>(t), s);
    return boost::lexical_cast<std::string>(t) + s;
  }
};

struct StringConcatLeft {
  typedef std::string result_type;
  boost::shared_ptr<std::vector<std::pair<std::string, std::string>>> calls;

  StringConcatLeft()
      : calls(boost::make_shared<
            std::vector<std::pair<std::string, std::string>>>()) {}

  template <typename T>
  std::string operator()(const std::string s, T t) const {
    calls->emplace_back(s, boost::lexical_cast<std::string>(t));
    return s + boost::lexical_cast<std::string>(t);
  }
};
}

namespace sboost {
void fusionfoldutilTests(stest::TestCollector& col) {
  col.addTest("sboost_fusionfoldr_foldr1", []()->void {
    boost::fusion::vector<std::string, int, unsigned, std::string> v(
        "Hi", 3, 9u, "Ho");
    StringConcatRight concat;
    BOOST_CHECK_EQUAL(sboost::FusionFoldUtil::foldr1(v, concat), "Hi39Ho");
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
    BOOST_CHECK_EQUAL(sboost::FusionFoldUtil::foldl1(v, concat), "Hi39Ho");

    std::vector<std::pair<std::string, std::string>> expectedCalls;
    expectedCalls.emplace_back("Hi", "3");
    expectedCalls.emplace_back("Hi3", "9");
    expectedCalls.emplace_back("Hi39", "Ho");
    BOOST_CHECK(*concat.calls == expectedCalls);
  });
}
};
