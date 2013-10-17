//@PURPOSE: Provide a runtime identifier for C++ types.
//
//@CLASSES:
//  CppTypeId: a runtime identifier for C++ types
//
//@DESCRIPTION:
// It is sometimes useful to be able to, at runtime, query and compare types.
// This class provides a typed means of doing so by wrapping typeid
// functionality.
//
// Usage
// -----
// This section illustrates indended use of this component.
//
// Example 1: Check the type of a boost::any object
// - - - - - - - - - - - - - - - - - - - - - - - -
//
//..
//  bool isIntType( const boost::any & a )
//  {
//    return CppTypeId::fromAny( a ) == CppTypeId::fromType<int>();
//  }
//..
#ifndef SMISC_CPPTYPEID_HPP_
#define SMISC_CPPTYPEID_HPP_

#include <string>

namespace boost {
class any;
}

namespace smisc {
// This class implements a runtime identifier for C++ types.
struct CppTypeId {

  // Return whether or not this type is equivelent to the specified 'typeId'.
  bool operator==(const CppTypeId& typeId) const;

  // Return 'this->toString() < a.toString()'.
  bool operator<(const CppTypeId& a) const;

  // Create a 'CppyTypeId' object from the specified 'Type'.
  template <typename Type>
  static CppTypeId fromType();

  // Create a 'CppyTypeId' object from the specified 'any'.
  static CppTypeId fromAny(const boost::any& any);

  // Return a unique human readable representation of this 'CppyTypeId'. Note
  // that the result of this function will vary between compilers.
  const std::string & toString() const;

 private:
  explicit CppTypeId(const std::string& id);

  std::string id;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
CppTypeId CppTypeId::fromType() {
  return CppTypeId(typeid(T).name());
}
};

#endif
