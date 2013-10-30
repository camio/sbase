#include <smisc/classserializer.hpp>

namespace smisc {
ClassSerializer::ClassSerializer(
    boost::function<std::string(boost::any)> toString_,
    boost::function<boost::any(std::string)> fromString_)
    : m_toString(toString_), m_fromString(fromString_) {}

std::string ClassSerializer::toString(boost::any val) const {
  return m_toString(val);
}

boost::any ClassSerializer::fromString(std::string string) const {
  return m_fromString(string);
}
}
