#ifndef STEST_TESTS_HPP_
#define STEST_TESTS_HPP_

//@PURPOSE: Collect all tests in the stest package.
//
//@FUNCTIONS: stest::tests
//
//@SEE_ALSO: stest::TestCollector

namespace stest {
  struct TestCollector;
}

namespace stest {
  void tests( stest::TestCollector & );
}

#endif
