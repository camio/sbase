#ifndef SMISC_TEXTOARCHIVEWITHANYSERIALIZER_HPP_
#define SMISC_TEXTOARCHIVEWITHANYSERIALIZER_HPP_

//@PURPOSE: Provide a text-based output archive with 'AnySerializer' support.
//
//@CLASSES:
//  smisc::TextOArchiveWithAnySerializer: saving archive with any support.
//
//@SEE_ALSO: smisc_anyserializer, smisc_textiarchivewithanyserializer
//
//@DESCRIPTION: This component provides 'TextOArchiveWithAnySerializer' which
// acts just like 'boost::text_oarchive' except the 'anySerializer' reference is
// also provided. This extra member allows certain objects to serialize which
// require an 'anySerializer'.
//
// Usage
// -----
// This section illustrates intended use of this component.

#include <boost/archive/text_oarchive.hpp>

namespace smisc {
struct AnySerializer;

// This class implements a 'Saving Archive Concept' that is equivelent to
// 'boost::text_oarchive' with the exception that an 'AnySerializer' reference
// is also provided.
struct TextOArchiveWithAnySerializer
    : boost::archive::text_oarchive_impl<TextOArchiveWithAnySerializer> {

  // Create a 'TextOArchiveWithAnySerializer' object with the specified
  // 'anySerializer' that will output to the specified 'outStream' using the
  // specified 'flags'. The reference to 'anySerializer' is held for the
  // lifetime of this object.
  TextOArchiveWithAnySerializer(const smisc::AnySerializer& anySerializer,
                        std::ostream& outStream,
                        unsigned int flags = 0);

  const smisc::AnySerializer& anySerializer;  // An 'AnySerializer' available to
                                              // Serializables for saving .
};
}
#endif
