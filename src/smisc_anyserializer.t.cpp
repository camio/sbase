#include <smisc/anyserializer.t.hpp>

#include <smisc/anyserializer.hpp>
#include <smisc/classserializerlexicalcastutil.hpp>
#include <stest/testcollector.hpp>

static void example1() {
  smisc::AnySerializer ser;
  ser.registerType<int>(
      "Int",
      smisc::ClassSerializerLexicalCastUtil::classSerializerFromLexicalCasts<
          int>());
  ser.registerType<double>(
      "Double",
      smisc::ClassSerializerLexicalCastUtil::classSerializerFromLexicalCasts<
          double>());
}

static std::pair<std::string, std::string> example2_anyToStringPair(
    smisc::AnySerializer ser,
    boost::any any) {
  const smisc::CppTypeId cppTypeId = smisc::CppTypeId::fromAny(any);
  if (!ser.hasCppTypeId(cppTypeId))
    throw std::runtime_error("Unsupported boost::any");
  else
    return std::make_pair(
        ser.entryFromCppTypeId(cppTypeId).uniqueTypeId,
        ser.entryFromCppTypeId(cppTypeId).opSerializer->toString(any));
}

static boost::any example2_stringPairToAny(
    smisc::AnySerializer ser,
    std::pair<std::string, std::string> pair) {

  if (!ser.hasUniqueTypeId(pair.first))
    throw std::runtime_error("Unsupported boost::any");
  else
    return ser.entryFromUniqueTypeId(pair.first)
        .opSerializer->fromString(pair.second);
}

namespace smisc {

void anyserializerTests(stest::TestCollector& col) {
  col.addTest("smisc_anyserializer_misc", []()->void {
    smisc::AnySerializer ser;
    ser.registerType<int>(
        "Int",
        smisc::ClassSerializerLexicalCastUtil::classSerializerFromLexicalCasts<
            int>());
    const std::pair<std::string, std::string> strings =
        example2_anyToStringPair(ser, boost::any(3));
    const boost::any result = example2_stringPairToAny(ser, strings);
    BOOST_CHECK_NO_THROW(boost::any_cast<int>(result));
    BOOST_CHECK_EQUAL(boost::any_cast<int>(result), 3);
  });
}
}
