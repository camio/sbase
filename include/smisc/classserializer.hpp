#ifndef SMISC_CLASSSERIALIZER_HPP_
#define SMISC_CLASSSERIALIZER_HPP_

//@PURPOSE: Provide a structure for serialization for individual types.
//
//@CLASSES:
//  smisc::ClassSerializer: string serialization routines for a type
//
//@SEE_ALSO: smisc_anyserializer, smisc_classserializerboostserializableutil,
//  smisc_classserializerlexicalcastutil, smisc_classserializerutil
//
//@DESCRIPTION: This component provides a single class, 'ClassSerializer', that
// includes string serialization routines for a particular type. It is mostly
// used as a means to build up serialization routines for multi-type
// containers, such as 'boost::any' (see smisc_anyserializer).
//
// Usage
// -----
// This section illustrates intended use of this component.

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <string>

namespace smisc {

// This class implements string serialization routines for a particular type.
struct ClassSerializer {
  // Create a 'ClassSerializer' object with the specified 'toString' and
  // 'fromString' functions. The behavior is undefined unless 'toString' and
  // 'fromString' are inverses for all values of a particular type.
  ClassSerializer(boost::function<std::string(boost::any)> toString,
                  boost::function<boost::any(std::string)> fromString);

  // Return the string associated with the specified 'value'. The behavior is
  // undefined unless 'value' has the type associated with this
  // 'ClassSerializer'
  std::string toString( boost::any value ) const;

  // Return the value associated with the specified 'string'. The behavior is
  // undefined unless 'string' is something that could be returned by the
  // 'toString' function.
  boost::any fromString( std::string string ) const;

private:
  boost::function<std::string(boost::any)> m_toString;
  boost::function<boost::any(std::string)> m_fromString;
};
}
#endif
