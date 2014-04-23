#ifndef SFRP_CACHEDPULL_HPP_
#define SFRP_CACHEDPULL_HPP_

//@PURPOSE: Provide a time/value pair for use in caching time functions.
//
//@CLASSES:
//  sfrp::CachedPull: time/value pair
//
//@DESCRIPTION: This component provides a single class, 'CachedPull', which is
// in essence just a time/value pair. It is designed to be used in components
// that have the need to cache the result of time value functions (aka.
// functions of type Time → Value for some type 'Value').
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Cache most recent call to a time function
// - - - - - - - - - - - - - - - - - - - - - - - - - -
// Say we have the following time function that we'd like to start caching.
//..
//  int someTimeFunction(double time);
//..
// We use a static 'CachedPull' object in the cached version of
// 'someTimeFunction'.
//..
// static int someTimeFunctionCached(double time) {
//   // We keep our cache in a static variable.
//   static boost::optional<sfrp::CachedPull<int>> previousPull;
//
//   // If the cache matches use it.
//   if (previousPull && previousPull->time() == time)
//     return previousPull->value();
//   else {
//     // Call 'someTimeFunction', set the cache, and return the result.
//     int result = someTimeFunction(time);
//     previousPull = sfrp::CachedPull<int>(time, result);
//     return result;
//   }
// }
//..

namespace sfrp {

// This class implements a pair of a time and a value of type 'Value'. It is
// particularly designed for use in caching the results of calling time
// functions.
template <typename Value>
struct CachedPull {
 public:
  // Create a new 'CachedPull' object with the specified 'time' and 'value'.
  CachedPull(double time, Value value);

  // Create a new 'CachedPull' object with the same value as the specified
  // 'other' 'CachedPull' object.
  CachedPull(CachedPull&& other);

  // Return the 'time' associated with this 'CachedPull' object.
  double time() const;

  // Return the 'value' associated with this 'CachedPull' object.
  const Value& value() const;

  // Give this 'CachedPull' object the same value as the specified 'other'
  // 'CachedPull' object.
  CachedPull& operator=(CachedPull&& other);

 private:
  double m_time;
  Value m_value;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename Value>
CachedPull<Value>::CachedPull(double time, Value value)
    : m_time(time), m_value(std::move(value)) {}

template <typename Value>
CachedPull<Value>::CachedPull(CachedPull&& other)
    : m_time(other.m_time), m_value(std::move(other.m_value)) {}

template <typename Value>
double CachedPull<Value>::time() const {
  return m_time;
}

template <typename Value>
const Value& CachedPull<Value>::value() const {
  return m_value;
}

template <typename Value>
CachedPull<Value>& CachedPull<Value>::operator=(CachedPull&& other) {
  m_time = other.m_time;
  m_value = std::move(other.m_value);
}
}
#endif
