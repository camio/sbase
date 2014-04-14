#ifndef SMISC_CLASSSERIALIZERBOOSTSERIALIZABLEUTIL_HPP_
#define SMISC_CLASSSERIALIZERBOOSTSERIALIZABLEUTIL_HPP_

//@PURPOSE: Provide for creation of 'ClassSerializers' from Serializables.
//
//@CLASSES:
//  smisc::ClassSerializerBoostSerializableUtil: utility namespace
//
//@SEE_ALSO: smisc_classserializer
//
//@DESCRIPTION: This component provides a single function,
// 'classSerializerFromSerializable', that can be used to generate
// 'ClassSerializer' objects based on types that fit Boost's Serializable
// concepts.
//
// Usage
// -----
// This section illustrates intended use of this component.

#include <sboost/serializablestringutil.hpp>
#include <smisc/classserializer.hpp>

namespace smisc {

// This class provides a namespace for operations dealing with both
// 'ClassSerializer' and boost's serializable concept.
struct ClassSerializerBoostSerializableUtil {

  // Return a 'ClassSerializer' that is constructed for the specified
  // 'Serializable' type using boost serialization routines. The behavior is
  // undefined unless the 'Serializable' type supports Boost's serializable
  // concept.
  template <typename Serializable>
  static smisc::ClassSerializer classSerializerFromSerializable();
};

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
struct ClassSerializerBoostSerializableUtil_TsfsSave {
  typedef std::string result_type;

  std::string operator()(const boost::any& a) const {
    T const* const tPtr = boost::any_cast<T>(&a);
    assert(tPtr);
    return sboost::SerializableStringUtil::toString(*tPtr);
  }
};

template <typename T>
struct ClassSerializerBoostSerializableUtil_TsfsLoad {
  typedef boost::any result_type;
  boost::any operator()(const std::string& s) const {
    const boost::optional<T> opT =
        sboost::SerializableStringUtil::fromString<T>(s);
    assert(opT);
    return boost::any(*opT);
  }
};

template <typename Serializable>
smisc::ClassSerializer
ClassSerializerBoostSerializableUtil::classSerializerFromSerializable() {
  return smisc::ClassSerializer(
      smisc::ClassSerializerBoostSerializableUtil_TsfsSave<Serializable>(),
      smisc::ClassSerializerBoostSerializableUtil_TsfsLoad<Serializable>());
}
}
#endif
