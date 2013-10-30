#include <smisc/anyserializer.hpp>

#include <cassert>
#include <utility>

namespace smisc {
AnySerializer::Entry::Entry(smisc::CppTypeId cti_,
                            std::string uniqueTypeId_,
                            boost::optional<ClassSerializer> opSerializer_)
    : cppTypeId(std::move(cti_)),
      uniqueTypeId(std::move(uniqueTypeId_)),
      opSerializer(std::move(opSerializer_)) {}

AnySerializer::AnySerializer() {}

void AnySerializer::registerType(
    smisc::CppTypeId cppTypeId,
    std::string uniqueTypeId,
    boost::optional<ClassSerializer> serializer) {
  typeTable.push_back(Entry(cppTypeId, uniqueTypeId, serializer));
}

bool AnySerializer::hasCppTypeId( const smisc::CppTypeId& cppTypeId ) const
{
  for (const Entry& entry : typeTable)
    if (entry.cppTypeId == cppTypeId)
      return true;
  return false;
}

bool AnySerializer::hasUniqueTypeId( const std::string& uniqueTypeId ) const
{
  for (const Entry& entry : typeTable)
    if (entry.uniqueTypeId == uniqueTypeId)
      return true;
  return false;
}

const AnySerializer::Entry& AnySerializer::entryFromCppTypeId(
    const smisc::CppTypeId& cppTypeId) const {
  for (const Entry& entry : typeTable)
    if (entry.cppTypeId == cppTypeId)
      return entry;
  assert(false);
  return *(Entry*)(0);
}

const AnySerializer::Entry& AnySerializer::entryFromUniqueTypeId(
    const std::string & uniqueTypeId) const {
  for (const Entry& entry : typeTable)
    if (entry.uniqueTypeId == uniqueTypeId)
      return entry;
  assert(false);
  return *(Entry*)(0);
}
}
