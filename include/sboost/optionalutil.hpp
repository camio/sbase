#ifndef SBOOST_OPTIONAL_HPP_
#define SBOOST_OPTIONAL_HPP_

//@PURPOSE: Provide utility operations for Boost.Optional objects.
//
//@CLASSES:
//  sboost::OptionalUtil_Join: optional 'join' template functor
//  sboost::OptionalUtil_HasValue: optional 'hasValue' template functor
//  sboost::OptionalUtil_GetValueOr: optional 'getValueOr' template functor
//  sboost::OptionalUtil_MapResult: 'map' result type computation
//  sboost::OptionalUtil: Boost.Optional utility operation namespace
//
//@DESCRIPTION: This component includes several classes that provide utilities
// that operate on Boost.Optional objects. The 'OptionalUtil' namespace class
// includes the most commonly used utilities.
//
// 'OptionalUtil_Join' and other template functor classes provide the same
// functions in 'OptionalUtil', but can be passed into higher order functions.
// This is required in more complecated libraries that use applicitive functors
// and similar constructs.
//
// The 'OptionalUtil_MapResult' function can be used to compute the return type
// of the 'map()' function. It is useful only to work with libraries that are
// compatible with the tr1 version of C++, but don't make use of 'decltype'
// from C++11.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Optional as error Monad
// - - - - - - - - - - - - - - - - -
//
// Making use of the 'bind()' function, we can use Boost.Optional as a monad
// that simulates early exit scenarios.
//
// Assume we have defined a function that will prompt the user for a string and
// return it if the process succeeded, otherwise it would return 'boost::none'.
//..
//  boost::optional<std::string> readString();
//..
// We can now use 'bind()' to make two calls to 'readString()' and return the
// concatenation of the two reads.
//..
//  const boost::optional<std::string> combinedStrings = sboost::OptionalUtil::bind(
//      readString(), [](const std::string & firstStringRead) {
//        return sboost::OptionalUtil::bind(
//            readString(),
//            [firstStringRead](const std::string & secondStringRead) {
//              return boost::make_optional(firstStringRead + secondStringRead);
//            });
//      });
//..
// The use of 'bind()' takes care of the checks that each read succeeds.
// 'combinedStrings' now holds either the concatenation of both the results or
// nothing if something went wrong with one of the reads. Note that if the
// first read fails, the second read is skipped.
//..
//  if (!combinedStrings)
//    std::cout << "There was an error reading at least one of the strings!"
//              << std::endl;
//  else
//    std::cout << "Your strings concatenated are: " << *combinedStrings
//              << std::endl;
//..

#include <boost/optional.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace sboost {

// This class provides a template functor implementation of the 'join()'
// function.
struct OptionalUtil_Join {
  // Return the result type of applying this functor with the specified
  // 'FunctorApplicationExpression'.
  template <typename FunctorApplicationExpression>
  struct result;

  // Return the value contained in the specified 'optionalOptionalValue' if it
  // is not none, otherwise return 'boost::none'.
  template <typename A>
  boost::optional<A> operator()(
      const boost::optional<boost::optional<A>>& optionalOptionalValue) const;
};

// This class provides a template functor implementation of the 'hasValue()'
// function.
struct OptionalUtil_HasValue {
  typedef bool result_type;

  // Return 'true' if the specified 'optionalValue' is not none, otherwise
  // return 'false'.
  template <typename T>
  inline bool operator()(const boost::optional<T>& optionalValue) const;
};

// This class provides a template functor implementation of the 'getValueOr()'
// function.
struct OptionalUtil_GetValueOr {
  // Return the result type of applying this functor with the specified
  // 'FunctorApplicationExpression'.
  template <typename FunctorApplicationExpression>
  struct result;

  // Return the value of the specified 'optionalValue' if it has one, otherwise
  // return the specified 'defaultValue'.
  template <typename A>
  A operator()(const boost::optional<A>& optionalValue,
               const A& defaultValue) const;
};

// Return the type of the result of the 'map' function assuming the
// specified 'F' is the type of its first argument and the specified
// 'A' is the type of its second argument.
template <typename F, typename A>
struct OptionalUtil_MapResult;

// This class provides utility operations for 'boost::optional' objects.
struct OptionalUtil {
  // Return the value contained in the specified 'optionalOptionalValue' if it
  // is not none, otherwise return 'boost::none'.
  template <typename T>
  static inline boost::optional<T> join(
      boost::optional<boost::optional<T>> optionalOptionalValue);

  // Return the specified 'primary' argument if it is not none, otherwise
  // return the specified 'secondary' value.
  template <typename A>
  static boost::optional<A> alternative(const boost::optional<A>& primary,
                                        const boost::optional<A>& secondary);

  // Return the value of the specified 'optionalValue' if it has one, otherwise
  // return the specified 'defaultValue'.
  template <typename A>
  static A getValueOr(const boost::optional<A>& optionalValue,
                      const A& defaultValue);

  // Return the result of applying the specified 'function' to the value within
  // the specified 'optionalValue' if 'optionalValue' is not none. Otherwise,
  // return 'boost::none'.
  template <typename F, typename A>
  static typename OptionalUtil_MapResult<F, A>::type map(
      F function,
      const boost::optional<A>& optionalValue);

  // Return the result of applying the specified 'function' to the value within
  // the specified 'optionalValue' if 'optionalValue' is not none. Otherwise,
  // return 'boost::none'.
  //
  // Note that the return type of 'function' must be 'boost::optional<B>' for
  // some type 'B'.
  template <typename F, typename A>
  static auto bind(const boost::optional<A>& optionalValue, F function)
      -> decltype(function(*optionalValue));
};
}

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================
namespace sboost {

template <typename A>
struct OptionalUtil_Join::result<OptionalUtil_Join(A)> {
  typedef typename A::value_type type;
};

template <typename A>
boost::optional<A> OptionalUtil_Join::operator()(
    const boost::optional<boost::optional<A>>& opOpA) const {
  return opOpA ? *opOpA : boost::none;
}

template <typename T>
bool OptionalUtil_HasValue::operator()(
    const boost::optional<T>& optionalValue) const {
  return optionalValue;
}

template <typename A, typename B>
struct OptionalUtil_GetValueOr::result<OptionalUtil_GetValueOr(A, B)> {
  typedef typename A::value_type type;
};

template <typename A>
A OptionalUtil_GetValueOr::operator()(const boost::optional<A>& optionalValue,
                                      const A& defaultValue) const {
  return optionalValue.get_value_or(defaultValue);
}

template <typename T>
boost::optional<T> OptionalUtil::join(
    boost::optional<boost::optional<T>> optionalOptionalValue) {
  return OptionalUtil_Join()(optionalOptionalValue);
}

template <typename A>
boost::optional<A> OptionalUtil::alternative(const boost::optional<A>& a,
                                             const boost::optional<A>& b) {
  return a ? a : b;
}

template <typename A>
A OptionalUtil::getValueOr(const boost::optional<A>& optionalValue,
                           const A& defaultValue) {
  return OptionalUtil_GetValueOr()(optionalValue, defaultValue);
}

template <typename F, typename A>
struct OptionalUtil_MapResult {
  typedef typename decltype((*(F*)(0))(*(A*)(0))) B;
  // We don't handle optional references and, since references
  // are out, remove any const qualifier.
  typedef typename boost::remove_reference<
      typename boost::remove_const<B>::type>::type BClean;
  typedef boost::optional<BClean> type;
};

template <typename F, typename A>
typename OptionalUtil_MapResult<F, A>::type OptionalUtil::map(
    F f,
    const boost::optional<A>& op) {
  return op ? boost::make_optional(f(*op)) : boost::none;
}

template <typename F, typename A>
auto OptionalUtil::bind(const boost::optional<A>& a, F f) -> decltype(f(*a)) {
  return a ? f(*a) : boost::none;
}
}

#endif
