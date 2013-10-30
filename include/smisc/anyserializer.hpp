#ifndef SMISC_ANYSERIALIZER_HPP_
#define SMISC_ANYSERIALIZER_HPP_

//@PURPOSE: Provide a datastructure for serializing 'boost::any' values.
//
//@CLASSES:
//  smisc::AnySerializer: datastructure for serializing 'boost::any' values.
//
//@SEE_ALSO: smisc_classserializer
//
//@DESCRIPTION: This component provides a single class, AnySerializer, which
// can be used both for the serialization of certain subsets of 'boost::any' as
// well as serialization of type names themselves.
//
// For the serialization of 'boost::any' subsets, one registers each supperted
// type with a unique identifier and a serializer. The unique identifier can be
// anything as long as now two types in a single 'AnySerializer' have the same
// one. The serializer must not be 'boost::none'.
//
// For serialization of types themselves, a serializer can be left as
// 'boost::none'.
//
// Usage
// -----
// This section illustrates intended use of this component.
//
// Example 1: Construction of an AnySerializer
// - - - - - - - - - - - - - - - - - - - - - -
// The serializers for the individually supported types are usually
// automatically generated via. a utility function. In this case we make use of
// the smisc_classserializerlexicalcastutil component, which uses
// 'boost::lexical_cast' as a means to generate the serializer functions.
//..
//  smisc::AnySerializer ser;
//  ser.registerType<int>(
//      "Int",
//      smisc::ClassSerializerLexicalCastUtil::classSerializerFromLexicalCasts<
//          int>());
//  ser.registerType<double>(
//      "Double",
//      smisc::ClassSerializerLexicalCastUtil::classSerializerFromLexicalCasts<
//          double>());
//..
//
// Example 2: Use of an AnySerializer to store/load values
// - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Use of an 'AnySerializer' usually consists of looking up an entry and making
// use of its serialization routines. Below we create a couple functions that
// convert 'boost::any' values to strings.
//
// Note that we create a pair of strings. The unique type identifier is useful
// for distinguishing between the types of stored 'any's.
//..
//  std::pair<std::string, std::string> anyToStringPair(
//      smisc::AnySerializer ser,
//      boost::any any) {
//    const smisc::CppTypeId cppTypeId = smisc::CppTypeId::fromAny(any);
//    if (!ser.hasCppTypeId(cppTypeId))
//      throw std::runtime_error("Unsupported boost::any");
//    else
//      return std::make_pair(
//          ser.entryFromCppTypeId(cppTypeId).uniqueTypeId,
//          ser.entryFromCppTypeId(cppTypeId).opSerializer->toString(any));
//  }
//..
// Here we go in the reverse direction in the straightforward way.
//..
//  boost::any stringPairToAny(
//      smisc::AnySerializer ser,
//      std::pair<std::string, std::string> pair) {
//  
//    if (!ser.hasUniqueTypeId(pair.first))
//      throw std::runtime_error("Unsupported boost::any");
//    else
//      return ser.entryFromUniqueTypeId(pair.first)
//          .opSerializer->fromString(pair.second);
//  }
//..
// Note that we are assuming that serializers exist for every registered type.
// This isn't going to be the case in general and needs to be handled properly.

#include <boost/optional.hpp>
#include <smisc/cpptypeid.hpp>
#include <smisc/classserializer.hpp>
#include <string>
#include <vector>

namespace smisc {

/** This data structure holds information about types adequate for
 *  serializing them, and optionally their instances, to/from file.
 */
// A serializer is only required for types, T, that need values,
// a ∈ T, to be serialized.

// This class implements a set of individual type serialization routines
// sufficient for serialization of a subset of 'boost::any' objects. Note that
// types can be registered without a serializer.
struct AnySerializer {

  // This class implements, optionally, a type serialization routine for a
  // single type along with type metadata.
  struct Entry {

    // Create an 'Entry' object with the specified 'cppTypeId', 'uniqueTypeId',
    // and 'optionalSerializer'. The behavior is undefined unless
    // 'optionalSerializer' is 'boost::none' or it supports the type with
    // identifier 'cppTypeId'.
    Entry(smisc::CppTypeId cppTypeId,
          std::string uniqueTypeId,
          boost::optional<ClassSerializer> optionalSerializer);

    smisc::CppTypeId cppTypeId;  // The C++ type identifier associated with the
                                 // type.
    std::string uniqueTypeId;    // A globally unique, platform independent,
                                 // indentifier for the type.
    boost::optional<ClassSerializer> opSerializer;  // An optional serializer
                                                    // for the type.
  };

  // Create a 'AnySerializer' object that doesn't recognize any types.
  AnySerializer();

  // Create a new entry with the specified 'cppTypeId', 'uniqueTypeId', and
  // 'serializer'. The behavior is undefined if 'cppTypeId' or 'uniqueTypeId'
  // has already been registered.
  void registerType(smisc::CppTypeId cppTypeId,
                    std::string uniqueTypeId,
                    boost::optional<ClassSerializer> serializer = boost::none);

  // Create a new entry with a 'cppTypeId' associated with the specified
  // 'Type', the specified 'uniqueTypeId', and the specified 'serializer'. The
  // behavior is undefined if 'Type' or 'uniqueTypeId' has already been
  // registered.
  template <typename Type>
  void registerType(std::string uniqueTypeId,
                    boost::optional<ClassSerializer> serializer = boost::none);

  // Return 'true' when this 'AnySerializer' has an entry for the specified
  // 'cppTypeId' and 'false' otherwise.
  bool hasCppTypeId(const smisc::CppTypeId& cppTypeId) const;

  // Return 'true' when this 'AnySerializer' has an entry for the specified
  // 'uniqueTypeId' and 'false' otherwise.
  bool hasUniqueTypeId(const std::string& uniqueTypeId) const;

  // Return the 'AnySerializer::Entry' corresponding to the specified
  // 'cppTypeId'. The behavior is undefined unless 'hasCppTypeId(cppTypeId) ==
  // true'.
  const Entry& entryFromCppTypeId(const smisc::CppTypeId& cppTypeId) const;

  // Return the 'AnySerializer::Entry' corresponding to the specified
  // 'uniqueTypeId'. The behavior is undefined unless
  // 'hasUniqueTypeId(uniqueTypeId) == true'.
  const Entry& entryFromUniqueTypeId(const std::string& uniqueTypeId) const;

 private:
  std::vector<Entry> typeTable;
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
void AnySerializer::registerType(std::string uniqueTypeId,
                                 boost::optional<ClassSerializer> serializer) {
  return registerType(
      smisc::CppTypeId::fromType<T>(), uniqueTypeId, serializer);
}
}
#endif
