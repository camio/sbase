#ifndef STEST_MASTERSUITETESTCOLLECTOR_HPP_
#define STEST_MASTERSUITETESTCOLLECTOR_HPP_

//@PURPOSE: A means to add tests to the Boost.Test master suite.
//
//@CLASSES: MasterSuiteTestCollector
//
//@SEE_ALSO: stest_TestCollector
//
//@DESCRIPTION: This component can be used as a means to connect tests defined
// using the TestCollector interface to the normal Boost.Test testing functions.
//
// Usage
// -----
//
// Example 1: Write a test application that runs tests on a particular package.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// For this example, we'll assume a ficticious 'example' package. We presume
// that the example package is following the testing conventions and has a
// component called tests that includes a tests function that takes in a
// reference to a stest::TestCollector.
//
// Note that all the defines must come before inclusion of
// 'stest/mastersuitetestcollector.hpp'. Otherwise a compilation error will
// likely result.
//..
//  // Renames the master test suite
//  #define BOOST_TEST_MODULE example test suite
//
//  // The alternative init api is preferred
//  #define BOOST_TEST_ALTERNATIVE_INIT_API
//
//  // Generates an empty test module initialization function
//  #define BOOST_TEST_MAIN
//
//  // Prevents the creation of a main() function. We define our own
//  // as a means of customization.
//  #define BOOST_TEST_NO_MAIN
//
//  // Include all of unit_test's component.
//  #include <boost/test/included/unit_test.hpp>
//
//  #include <example/test.hpp>
//  #include <stest/mastersuitetestcollector.hpp>
//
//
//  static bool registerTests()
//  {
//    example::tests( stest::MasterSuiteTestCollector() );
//    return true;
//  }
//
//  int main( int argc, char** argv )
//  {
//      return boost::unit_test::unit_test_main
//          ( &registerTests
//          , argc
//          , argv
//          );
//  }
//..

#include <stest/testcollector.hpp>

namespace stest {
// A test collector that adds all tests to the Boost.Test master test suite.
struct MasterSuiteTestCollector : TestCollector {
 private:
  void handleAddTest(boost::unit_test::test_case* const) override;
};
}

#endif
