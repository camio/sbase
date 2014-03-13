#ifndef SMISC_UTF8UTIL_HPP_
#define SMISC_UTF8UTIL_HPP_

//@PURPOSE: Provide utility operations covering UTF-8 encodings.
//
//@CLASSES:
//  smisc::Utf8Util: UTF-8 utility function namespace class
//
//@DESCRIPTION: This component provides a single namespace class, 'Utf8Util',
// which provides functions related to UTF-8 encoding. 
//
// Although there are many possible uses for this component, the intended use
// case is use of the 'length' function for achieving consistant indentation
// when used with a fixed width font. Note that 'std::strings's 'length'
// function doesn't serve this purpose because it is counting the number of
// bytes instead of the number of glyphs.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Output strings in two columns
// - - - - - - - - - - - - - - - - - - - -
// In this example we create a function that can format two strings such that
// one, when outputted, would look as if it is on a left column while the other
// shows on a right column.
//
// Assuming these are UTF-8 encoded strings, we need to figure out the length
// of the 'left' string in order to determine how many spaces are inserted in
// between.
//
//..
//  // Return a string where the specified 'left' string is left justified
//  // and the specified 'right' string begins in column 40. The behavior is
//  // undefined unless 'left' has a viewable length less 40.
//  std::string formatTwoColumns(const std::string left,
//                               const std::string right) {
//  {
//    std::string result = left;
//    const std::size_t numSpaces = 40 - smisc::Utf8Util::length(left);
//    for( std::size_t i = 0; i < numSpaces; ++i )
//      result += " ";
//    result += right;
//    return result;
//  }
//..

#include <string>

namespace smisc {

// This class provides a namespace for utility operations dealing with UTF-8
// encodings.
struct Utf8Util {
  // Return 'true' if the specified 'byte' is a one-byte code in UTF-8
  // encoding, otherwise return 'false'. Note that one-byte codes correspond to
  // ASCII values 0 through 127.
  static bool isOneByteCode(char byte);

  // Return 'true' if the specified 'byte' is the leading byte of a UTF-8
  // multibyte sequence, otherwise return 'false'. Note that leading bytes have
  // the two most significant binary digits as '1' and do not have the first 7
  // significant binary digits as all '1's.
  static bool isLeadingByte(char byte);

  // Return 'true' if the specified 'byte' is a continuation byte of a UTF-8
  // multibyte sequence, otherwise return 'false'. Note that continuation bytes
  // have the most significant binary digit as '1' and the second most
  // significant binary digit as '0'.
  static bool isContinuationByte(char byte);

  // Return the number of UTF-8 glyphs in the specified 'utf8String'. If
  // 'utf8String' is not a valid UTF-8 string, this function will return the
  // number of bytes that are either UTF-8 one byte codes or UTF-8 leading
  // bytes.
  static std::size_t length(const std::string& utf8String);
};
}

#endif
