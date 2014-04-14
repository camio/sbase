#include <scpp/pairutil.t.hpp>

#include <scpp/pairutil.hpp>
#include <stest/testcollector.hpp>
#include <string>

static std::string exampleFunction(int i, std::string s) {
  return s + std::to_string(i);
}

static void example() {
  auto exampleFunctionFused = scpp::PairUtil::fuse(exampleFunction);
  const std::string result = exampleFunctionFused(std::make_pair(3, "hello"));
}

static std::string testF(int i, std::string c) { return c; }

namespace scpp {
void pairutilTests(stest::TestCollector& col) {
  col.addTest("scpp_pairutil_swap", []()->void {
    BOOST_CHECK(scpp::PairUtil_Swap()(std::make_pair(3, 'c')) ==
                std::make_pair('c', 3));
  });
  col.addTest("scpp_pairutil_fused", []()->void {
    typedef std::string (*FunctionType)(int, std::string);
    PairUtil_FusedFunction<FunctionType> foo(testF);
    BOOST_CHECK_EQUAL(foo(std::make_pair(33, std::string("hello"))), "hello");

    // Check implicit conversion
    BOOST_CHECK_EQUAL(foo(std::make_pair(33, "hello")), "hello");

    // Check lambda functions
    BOOST_CHECK_EQUAL(PairUtil::fuse([](int i, std::string s) { return s; })(
                          std::make_pair(4, "hello world")),
                      "hello world");
  });
  col.addTest("scpp_pairutil_makePair", []()->void {
    BOOST_CHECK(scpp::PairUtil::makePair(3, std::string("foo")) ==
                std::make_pair(3, std::string("foo")));
    BOOST_CHECK_EQUAL(
        scpp::PairUtil::first(std::make_pair(3, std::string("foo"))), 3);
    BOOST_CHECK_EQUAL(
        scpp::PairUtil::second(std::make_pair(3, std::string("foo"))), "foo");
  });
}
}
