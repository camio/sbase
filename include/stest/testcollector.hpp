#ifndef STEST_TESTCOLLECTOR_HPP_
#define STEST_TESTCOLLECTOR_HPP_

//@PURPOSE: Provide an interface for collecting tests in a component or packages
//
//@CLASSES:
//  stest::TestCollector: consumer of unit test functions
//
//@SEE_ALSO: stest_MasterSuiteTestCollector
//
//@DESCRIPTION: This component can be used to create collection functions that
// allow for a manual registration of unittests. This is conventionally done at
// the component level, the package level, and the package group level with
// conventions defined in the examples below.
//
// Usage
// -----
//
// Example 1: Tests defined for a particular component.
// - - - - - - - - - - - - - - - - - - - - - - - - - -
// Lets say we are writing tests for an Optional type that is similar to the
// Boost.Optional library. Lets say component is called optional and it is in
// the 'example' package. Here is the header that defines the 'optionalTests'
// function that collects test cases (include/example/optional.hpp).
//
//..
//  #ifndef EXAMPLE_OPTIONAL_HPP_
//  #define EXAMPLE_OPTIONAL_HPP_
//
//  namespace stest {
//    struct TestCollector;
//  }
//
//  namespace example {
//    template< typename T >
//    class Optional
//    {
//    public:
//      Optional();
//      Optional( T );
//      bool operator==( Optional<T> ) const;
//      bool operator!=( Optional<T> ) const;
//      bool hasValue( ) const;
//      // @PRECONDITION: hasValue( ) == true
//      T getValue( ) const;
//    private:
//      bool m_hasValue;
//      T m_value;
//    };
//
//    void optionalTests( stest::TestCollector & );
//  }
//
//  #endif
//..
//
// The cpp file (src/example_optional.cpp), defines unit tests from within the
// optionalTests function.
//
//..
//  #include <example/optional.hpp>
//
//  #include <stest/testcollector.hpp>
//
//  namespace example {
//    //..
//    void optionalTests( stest::TestCollector & col )
//    {
//      col.addTest
//        ( "example_optional_equality"
//        , []() -> void
//          {
//            BOOST_CHECK( Optional<int>( 12 ) == Optional<int>( 12 ) );
//            BOOST_CHECK( Optional<int>( 12 ) != Optional<int>( 14 ) );
//            BOOST_CHECK( Optional<int>( 12 ) != Optional<int>( ) );
//          }
//        );
//      col.addTest
//        ( "example_optional_getValue"
//        , []() -> void
//          {
//            BOOST_CHECK( Optional<int>( 12 ).hasValue( ) );
//            BOOST_CHECK( Optional<int>( 12 ).getValue( ) == 12 );
//            BOOST_CHECK( !Optional<int>( ).hasValue( ) );
//          }
//        );
//    }
//  }
//..
//
// Here we defined two tests. The convention is to use
// "<package_name>_<component_name>_<namedtest>" as the names of the test cases.
//
/// Example 2: Tests defined for a particular package.
/// - - - - - - - - - - - - - - - - - - - - - - - - -
// Each package should create a component called 'test' that has a test
// collection function called tests. Consider the header for the example test
// component.
//
//..
//  #ifndef EXAMPLE_TESTS_HPP_
//  #define EXAMPLE_TESTS_HPP_
//
//  namespace stest {
//    struct TestCollector;
//  }
//
//  namespace example {
//    void tests( stest::TestCollector & );
//  }
//  #endif
//..
//
//  The cpp file will call the test collection function for every component in
//  the package.
//
//..
//  #include <example/tests.hpp>
//
//  #include <example/optional.hpp>
//  #include <example/tuple.hpp>
//
//  namespace example {
//    void tests( stest::TestCollector & col )
//    {
//      optionalTests( col );
//      tupleTests( col );
//      //...
//    }
//  }
//..
//
/// Example 3: Tests defined for a particular package group.
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Every package group should have a tests package and tests component
// declaring all the tests for that group. Lets say the 'example' component was
// part of the 'ex' package group. We would have a 'extest' package with a
// 'tests' component. The extest/tests.hpp would be as follows.
//..
//  #ifndef EXTEST_TESTS_HPP_
//  #define EXTEST_TESTS_HPP_
//
//  namespace stest {
//    struct TestCollector;
//  }
//
//  namespace extest {
//    void tests( stest::TestCollector & );
//  }
//  #endif
//..
//
// The cpp file would call the tests function for every package.
//
//..
//  #include <extest/tests.hpp>
//
//  #include <example/tests.hpp>
//  //...
//
//  namespace extest {
//    void tests( stest::TestCollector & col )
//    {
//      example::tests( col );
//    }
//  }

#include <boost/test/unit_test.hpp>

namespace stest {
struct TestCollector {
  // This function is called by the user of TestCollector to register a
  // new test. If name is null, the results of this call are undefined.
  template <typename F>
  void addTest(char* const name, F f) {
    handleAddTest(boost::unit_test::make_test_case(
        boost::unit_test::callback0<>(f), name));
  }

 protected:
  // Child classes are expected to reimplement this function to customize
  // the functionality of this TestCollector.
  virtual void handleAddTest(boost::unit_test::test_case* const) = 0;
};
}

#endif
