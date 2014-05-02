#ifndef SBOOST_SERIALIZABLESTRINGUTIL_HPP_
#define SBOOST_SERIALIZABLESTRINGUTIL_HPP_

//@PURPOSE: Provide conversions to/from strings using Serializable objects.
//
//@CLASSES:
//  sboost::SerializableStringUtil: namespace for string conversion functions
//
//@DESCRIPTION: This component provides functions that convert serializable
// objects to strings and vice-versa. These functions can be used as debugging
// aids or to build up other specialized utilities.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Clone Function
// - - - - - - - - - - - - -
// To build a deep clone function for objects that don't have special 'clone()'
// methods, we can take advantage of serialization methods if they are already
// implemented.
//..
//  template< typename T >
//  static T clone( const T & t ) {
//    return *sboost::SerializableStringUtil::fromString<T>(
//        sboost::SerializableStringUtil::toString(t));
//  }
//..

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/optional.hpp>
#include <sstream>
#include <string>

namespace sboost {

// This class provides a namespace for operations that convert serializable
// values to/from representations as 'std::string' objects.
struct SerializableStringUtil {
  // Return a string representation of the specified 'value'. The behavior is
  // undefined unless 'value' meets the Serializable concept of
  // Boost.Serialize.
  template <typename Serializable>
  static std::string toString(const Serializable& value);

  // Parse the specified 'data' string into the specified 'Serializable' type.
  // Return the value retrieved if parsing was a success, otherwise return
  // 'boost::none'. The behavior is undefined unless 'Serializable' meeds the
  // Serializable concept of Boost.Serialize.
  template <typename Serializable>
  static boost::optional<Serializable> fromString(const std::string& data);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
std::string SerializableStringUtil::toString(const T& a) {
  std::ostringstream oss;
  boost::archive::text_oarchive oa(oss);
  oa << a;
  return oss.str();
}

template <typename T>
boost::optional<T> SerializableStringUtil::fromString(const std::string& a) {
  std::istringstream iss(a);
  boost::archive::text_iarchive ia(iss);
  T t;
  try {
    ia >> t;
    return boost::make_optional(t);
  }
  catch (boost::archive::archive_exception&) {
    return boost::none;
  }
}
}
#endif
