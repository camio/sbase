#include <smisc/utf8util.hpp>

namespace smisc {

bool Utf8Util::isOneByteCode(char byte) { return (byte & 0x80) == 0; }

bool Utf8Util::isLeadingByte(char byte) {
  return ((byte & 0xC0) == 0xC0) && ((byte & 0xFE) != 0xFE);
}

bool Utf8Util::isContinuationByte(char byte) { return (byte & 0xC0) == 0x80; }

std::size_t Utf8Util::length(const std::string& utf8String) {
  std::size_t result = 0;
  for (char c : utf8String)
    if (isOneByteCode(c) || isLeadingByte(c))
      ++result;
  return result;
}
}
