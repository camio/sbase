#ifndef SBASETEST_TESTS_HPP_
#define SBASETEST_TESTS_HPP_

//@PURPOSE: Collect all tests in the sbase package group.
//
//@FUNCTIONS: sbasetest::tests
//
//@SEE_ALSO: stest::TestCollector

namespace stest {
  struct TestCollector;
}

namespace sbasetest {
  void tests( stest::TestCollector & col );
}

#endif
