#include <smisc/cpptypeid.hpp>

#include <boost/any.hpp>

namespace smisc {

CppTypeId::CppTypeId(const std::string& id_) : id(id_) {}

bool CppTypeId::operator==(const CppTypeId& a) const { return id == a.id; }

bool CppTypeId::operator<(const CppTypeId& a) const { return id < a.id; }

const std::string& CppTypeId::toString() const { return id; }

CppTypeId CppTypeId::fromAny(const boost::any& a) {
  return CppTypeId(a.type().name());
}
}
