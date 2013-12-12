#ifndef SCPP_VECTORUTIL_HPP_
#define SCPP_VECTORUTIL_HPP_

//@PURPOSE: Provide utility functions for 'std::vector' objects.
//
//@CLASSES:
//  scpp::VectorUtil: 'std::vector' utility function namespace
//
//@DESCRIPTION: This component provides a single struct, 'VectorUtil', that
// acts as a namespace for 'std::vector' utility operations.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Add an element to the vector if it doesn't already have one.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//..
//  static void pushBackIfUnique(std::vector<std::string>& vector,
//                               const std::string s) {
//    if (!scpp::VectorUtil::hasElement(vector, s))
//      vector.emplace_back(std::move(s));
//  }
//..

#include <vector>
#include <algorithm>

namespace scpp {

// This class provides a namespace for utility operations using a
// 'std::vector' object.
struct VectorUtil {

  // Return 'true' if the specified 'vector' has an entry with the specified
  // 'value', otherwise 'false'.
  template <typename T, typename U>
  static bool hasElement(const std::vector<T>& vector, const U& value);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T, typename U>
bool VectorUtil::hasElement(const std::vector<T>& vector, const U& value) {
  const std::vector<std::string>::const_iterator i =
      std::find(vector.begin(), vector.end(), value);
  return i != vector.end();
}
}
#endif
