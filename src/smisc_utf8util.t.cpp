#include <smisc/utf8util.t.hpp>

#include <smisc/utf8util.hpp>
#include <stest/testcollector.hpp>
#include <string>

// Return a string where the specified 'left' string is justified to the left
// and the specified 'right' string begins in column 40. The behavior is
// undefined unless 'left' has a viewable length less 40.
static std::string formatTwoColumns(const std::string left,
                                    const std::string right) {
  std::string result = left;
  const std::size_t numSpaces = 40 - smisc::Utf8Util::length(left);
  for( std::size_t i = 0; i < numSpaces; ++i )
    result += " ";
  result += right;
  return result;
}

namespace smisc {

void utf8utilTests(stest::TestCollector& col)
{
  col.addTest("smisc_utf8util_isOneByteCode", []()->void {
    BOOST_CHECK(Utf8Util::isOneByteCode('a'));
    BOOST_CHECK(Utf8Util::isOneByteCode('0'));
    BOOST_CHECK(Utf8Util::isOneByteCode('z'));
    // 0x3F = 01111111
    BOOST_CHECK(Utf8Util::isOneByteCode(0x3F));

    // 0x80 = 10000000
    BOOST_CHECK(!Utf8Util::isOneByteCode(static_cast<unsigned char>(0x80)));

    // ∀ is translated into three characters in the UTF-8 encoding.
    BOOST_CHECK(!Utf8Util::isOneByteCode(std::string("∀")[0]));
    BOOST_CHECK(!Utf8Util::isOneByteCode(std::string("∀")[1]));
    BOOST_CHECK(!Utf8Util::isOneByteCode(std::string("∀")[2]));
    // 0xFE is the one character that isn't defined in the UTF-8 encoding.
    BOOST_CHECK(!Utf8Util::isOneByteCode(static_cast<unsigned char>(0xFE)));
  });
  col.addTest("smisc_utf8util_isLeadingByte", []()->void {

    // ∀ has a 3 byte UTF-8 Encoding
    BOOST_CHECK(Utf8Util::isLeadingByte(std::string("∀")[0]));

    // Ā has a 2 byte UTF-8 Encoding
    BOOST_CHECK(Utf8Util::isLeadingByte(std::string("Ā")[0]));

    // 0xFE is the one character that isn't defined in the UTF-8 encoding.
    BOOST_CHECK(!Utf8Util::isLeadingByte(static_cast<unsigned char>(0xFE)));

    BOOST_CHECK(!Utf8Util::isLeadingByte('a'));
  });
  col.addTest("smisc_utf8util_isContinuationByte", []()->void {
    // 0xFE is the one character that isn't defined in the UTF-8 encoding.
    BOOST_CHECK(
        !Utf8Util::isContinuationByte(static_cast<unsigned char>(0xFE)));

    BOOST_CHECK(!Utf8Util::isContinuationByte(std::string("∀")[0]));
    BOOST_CHECK(Utf8Util::isContinuationByte(std::string("∀")[1]));
    BOOST_CHECK(Utf8Util::isContinuationByte(std::string("∀")[2]));

    BOOST_CHECK(!Utf8Util::isContinuationByte(std::string("Ā")[0]));
    BOOST_CHECK(Utf8Util::isContinuationByte(std::string("Ā")[1]));

    BOOST_CHECK(!Utf8Util::isContinuationByte('a'));
  });
  col.addTest("smisc_utf8util_length", []()->void {
    BOOST_CHECK_EQUAL(Utf8Util::length("hello world"), 11);
    BOOST_CHECK_EQUAL(Utf8Util::length("hello ∀ that is ⊥"), 17);

    // Set 'brokenUtf8' to one leading byte and one space.
    std::string brokenUtf8;
    brokenUtf8 += char(0xc0);
    brokenUtf8 += " ";
    BOOST_CHECK_EQUAL(Utf8Util::length(brokenUtf8), 2);

    // Set 'brokenUtf8' to two continuation bytes
    brokenUtf8 = "";
    brokenUtf8 += char(0x80);
    brokenUtf8 += char(0x80);
    BOOST_CHECK_EQUAL(Utf8Util::length(brokenUtf8), 0);
  });
}
}
