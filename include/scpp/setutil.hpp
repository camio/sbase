#ifndef SCPP_SETUTIL_HPP_
#define SCPP_SETUTIL_HPP_

//@PURPOSE: Provide utility functions for 'std::set' objects.
//
//@CLASSES:
//  scpp::SetUtil: 'std::set' utility function namespace
//
//@DESCRIPTION: This component provides a single struct, 'SetUtil', that
// acts as a namespace for 'std::set' utility operations.
//
// Usage
// -----
// This section illustrates intended use of this component.

#include <set>
#include <algorithm>

namespace scpp {

// This class provides a namespace for utility operations using a
// 'std::set' object.
struct SetUtil {

  // Return a set of all the elements that exist in the specified 'set' that are
  // not in the specified 'subtractorSet'.
  template <typename T>
  static std::set<T> difference(const std::set<T>& set,
                                const std::set<T>& subtractorSet);

  // Return 'true' if the specified 'set' contains the specified 'value',
  // otherwise 'false'.
  template <typename T, typename U>
  static bool contains(const std::set<T>& set, const U& value);
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================
template <typename T>
std::set<T> SetUtil::difference(const std::set<T>& s0, const std::set<T>& s1) {
  std::set<T> r;
  std::set_difference(
      s0.begin(), s0.end(), s1.begin(), s1.end(), std::inserter(r, r.begin()));
  return r;
}
template <typename T, typename U>
bool SetUtil::contains(const std::set<T>& s0, const U& t) {
  return s0.find(t) != s0.end();
}
}
#endif
