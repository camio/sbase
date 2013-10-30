#ifndef SMISC_CLASSSERIALIZERUTIL_HPP_
#define SMISC_CLASSSERIALIZERUTIL_HPP_

//@PURPOSE: Provide utility operations for 'ClassSerializer' objects.
//
//@CLASSES:
//  smisc::ClassSerializerUtil: namespace for utility operations
//
//@SEE_ALSO: smisc_classserializer
//
//@DESCRIPTION: This component provides utility functions for 'ClassSerializer'
// objects.
//
// Usage
// -----
// This section illustrates intended use of this component.

#include <smisc/classserializer.hpp>

namespace smisc {

// This class provides a namespace for utility operations using a
// 'ClassSerializer' object.
struct ClassSerializerUtil {

  // Return a ClassSerializer whose 'toString' function always returns "" and
  // whose 'fromString' function always returns a 'boost::any' containing the
  // specified 'unitValue'.
  template <typename UnitType>
  static smisc::ClassSerializer unitSerializer(const UnitType& unitValue);
};


// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename UnitType>
static smisc::ClassSerializer ClassSerializerUtil::unitSerializer(
    const UnitType& unitValue)
{
  return smisc::ClassSerializer(
      ([](const boost::any&) { return std::string(""); }),
      ([unitValue](const std::string&) { return boost::any(unitValue); }));
}
}
#endif
