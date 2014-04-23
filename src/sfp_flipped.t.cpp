#include <sfp/flipped.t.hpp>

#include <sfp/flipped.hpp>
#include <stest/testcollector.hpp>
#include <functional>  // std::minus
#include <string>

namespace {

void example(){
  typedef sfp::Flipped<std::minus<int>> Subtract;
  const int one = Subtract()(3, 4);
}

int testFunc( int i, std::string s ) {
  return i;
}

struct TestFunctor
{
  std::string operator()( int i, std::string j ) const {
    return "First";
  }
  int operator()( std::string j, int i ) const {
    return 2;
  }
};
}

namespace sfp {
void flippedTests(stest::TestCollector& col)
{
  col.addTest("sfp_flipped_misc", []()->void {
    typedef int (*FuncType)(int i, std::string s);
    Flipped<FuncType> flippedTestFunc(testFunc);
    BOOST_CHECK_EQUAL(flippedTestFunc("hello", 3), 3);

    // Check that a polymorphic functor retains its polymorphism.
    Flipped<TestFunctor> flippedTestFunctor =
        Flipped<TestFunctor>(TestFunctor());

    BOOST_CHECK_EQUAL(flippedTestFunctor(3, "hello"), 2);
    BOOST_CHECK_EQUAL(flippedTestFunctor("hello", 3), "First");

    // Check that lambda functions work.
    const auto f = [](int, std::string s) { return s; };
    Flipped<decltype(f)> flippedLambda(f);
    BOOST_CHECK_EQUAL(flippedLambda("three", 3), "three");
  });
  col.addTest("sfp_flipped_example", []()->void {
    typedef sfp::Flipped<std::minus<int>> Subtract;
    BOOST_CHECK_EQUAL( Subtract()(3, 4), 1 );
  });
}
}
