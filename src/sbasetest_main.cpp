// Renames the master test suite
#define BOOST_TEST_MODULE sbase test suite

// The alternative init api is preferred
#define BOOST_TEST_ALTERNATIVE_INIT_API

// Generates an empty test module initialization function
#define BOOST_TEST_MAIN

// Prevents the creation of a main() function. We define our own
// as a means of customization.
#define BOOST_TEST_NO_MAIN

// Include all of unit_test's component.
#include <boost/test/included/unit_test.hpp>

#include <sbasetest/tests.hpp>
#include <stest/mastersuitetestcollector.hpp>
#include <iostream>


static bool registerTests() {
  sbasetest::tests(stest::MasterSuiteTestCollector());
  return true;
}

int main(int argc, char** argv) {
  return boost::unit_test::unit_test_main(&registerTests, argc, argv);
}
