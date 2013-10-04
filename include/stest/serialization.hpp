#ifndef STEST_SERIALIZATION_HPP_
#define STEST_SERIALIZATION_HPP_

//@PURPOSE: Provide tools that simplify Boost.Serilization unittest creation.
//
//@FUNCTIONS: serializableRoundtrip
//
//@SEE_ALSO: TestCollector
//
//@DESCRIPTION: This component houses utilities that make it easier to write
// unittests for objects that fit the Serializable concept in the
// Boost.Serialization library.
//
// Usage
// -----
//
// Example 1: Unittest for round-trip accuracy
// - - - - - - - - - - - - - - - - - - - - -
// Here, we are testing that serialization for boost::optional<int> works
// correctly.
//..
//  #include <boost/test/unit_test.hpp>
//  #include <boost/optional.hpp>
//  #include <boost/serialization/optional.hpp>
//  #include <stest/serialization.hpp>
//  #include <stest/testcollector.hpp>
//
//  namespace boost {
//    void boostTests( stest::TestCollector & col )
//    {
//      col.addTest
//        ( "boost_optional_serialization"
//        , []() -> void
//          {
//            BOOST_CHECK( stest::serializableRoundtrip( boost::optional<int>( 4
// ) ) );
//            BOOST_CHECK( stest::serializableRoundtrip( boost::optional<int>( )
// ) );
//          }
//        );
//    }
//..

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

namespace stest {
template <typename Serializable>
bool serializableRoundtrip(Serializable value) {
  std::ostringstream oss;
  boost::archive::xml_oarchive oa(oss);
  oa << boost::serialization::make_nvp("test", value);

  Serializable value2;
  std::istringstream iss(oss.str());
  boost::archive::xml_iarchive ia(iss);
  ia >> boost::serialization::make_nvp("test", value2);

  return value == value2;
}
}

#endif
