#ifndef SMISC_TEXTIARCHIVEWITHANYSERIALIZER_HPP_
#define SMISC_TEXTIARCHIVEWITHANYSERIALIZER_HPP_

//@PURPOSE: Provide a text-based input archive with 'AnySerializer' support.
//
//@CLASSES:
//  smisc::TextIArchiveWithAnySerializer: loading archive with any support.
//
//@SEE_ALSO: smisc_anyserializers, misc_textoarchivewithanyserializer
//
//@DESCRIPTION: This component provides 'TextIArchiveWithAnySerializer' which
// acts just like 'boost::text_iarchive' except the 'anySerializer' reference is
// also provided. This extra member allows certain objects to serialize which
// require an 'anySerializer'.
//
// Usage
// -----
// This section illustrates intended use of this component.

#include <boost/archive/text_iarchive.hpp>

namespace smisc {
struct AnySerializer;

// This class implements a 'Loading Archive Concept' that is equivelent to
// 'boost::text_iarchive' with the exception that an 'AnySerializer' reference
// is also provided.
struct TextIArchiveWithAnySerializer
    : boost::archive::text_iarchive_impl<TextIArchiveWithAnySerializer> {

  // Create a 'TextIArchiveWithAnySerializer' object with the specified
  // 'anySerializer' that will input from the specified 'inStream' using the
  // specified 'flags'. The reference to 'anySerializer' is held for the
  // lifetime of this object.
  TextIArchiveWithAnySerializer(const smisc::AnySerializer& anySerializer,
                                std::istream& inStream,
                                unsigned int flags = 0);

  const smisc::AnySerializer& anySerializer;  // An 'AnySerializer' available to
                                              // Serializables for loading.
};
}
#endif
