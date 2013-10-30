#ifndef SMISC_CLASSSERIALIZERLEXICALCASTUTIL_HPP_
#define SMISC_CLASSSERIALIZERLEXICALCASTUTIL_HPP_

//@PURPOSE: Provide for creation of 'ClassSerializers' using Boost.LexicalCast.
//
//@CLASSES:
//  smisc::ClassSerializerLexicalCastUtil: utility namespace
//
//@SEE_ALSO: smisc_classserializer
//
//@DESCRIPTION: This component provides a single function,
// 'classSerializerFromLexicalCasts', that can be used to generate
// 'ClassSerializer' objects using 'boost::lexical_cast'. Note that the
// 'boost::lexical_cast' calls must be properly inverted in order to meet the
// invariants of 'classSerializerFromLexicalCasts'.
//
// Usage
// -----
// This section illustrates intended use of this component.

#include <boost/lexical_cast.hpp>
#include <smisc/classserializer.hpp>

namespace smisc {
// This class provides a namespace for operations dealing with both
// 'ClassSerializer' and boost's lexical_cast library.
struct ClassSerializerLexicalCastUtil {

  // Return a 'ClassSerializer' that is constructed for the specified
  // 'LexicalCastable' type using boost lexical_cast routines. The behavior is
  // undefined unless the 'LexicalCastable' type supports lexical_cast and
  // lexical_cast's string functions are inverse of each other.
  template <typename LexicalCastable>
  static smisc::ClassSerializer classSerializerFromLexicalCasts();
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
smisc::ClassSerializer
ClassSerializerLexicalCastUtil::classSerializerFromLexicalCasts() {
  return smisc::ClassSerializer(
      ([](boost::any a) {
        return boost::lexical_cast<std::string>(boost::any_cast<T>(a));
      }),
      ([](std::string s) { return boost::any(boost::lexical_cast<T>(s)); }));
}
}

#endif
