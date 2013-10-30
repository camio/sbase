#include <smisc/textoarchivewithanyserializer.hpp>

#include <boost/archive/impl/archive_serializer_map.ipp>
#include <boost/archive/impl/basic_text_oarchive.ipp>
#include <boost/archive/impl/text_oarchive_impl.ipp>

namespace boost {
namespace archive {

template class detail::archive_serializer_map<
    smisc::TextOArchiveWithAnySerializer>;
template class basic_text_oarchive<smisc::TextOArchiveWithAnySerializer>;
template class text_oarchive_impl<smisc::TextOArchiveWithAnySerializer>;
}
}

namespace smisc {

TextOArchiveWithAnySerializer::TextOArchiveWithAnySerializer(
    const smisc::AnySerializer& anySerializer_,
    std::ostream& os,
    unsigned int flags)
    : boost::archive::text_oarchive_impl<TextOArchiveWithAnySerializer>(os,
                                                                        flags),
      anySerializer(anySerializer_) {}
}
