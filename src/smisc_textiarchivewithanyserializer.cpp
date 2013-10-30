#include <smisc/textiarchivewithanyserializer.hpp>

#include <boost/archive/impl/archive_serializer_map.ipp>
#include <boost/archive/impl/basic_text_iarchive.ipp>
#include <boost/archive/impl/text_iarchive_impl.ipp>

namespace boost {
namespace archive {

template class detail::archive_serializer_map<
    smisc::TextIArchiveWithAnySerializer>;
template class basic_text_iarchive<smisc::TextIArchiveWithAnySerializer>;
template class text_iarchive_impl<smisc::TextIArchiveWithAnySerializer>;
}
}

namespace smisc {

TextIArchiveWithAnySerializer::TextIArchiveWithAnySerializer(
    const smisc::AnySerializer& d_,
    std::istream& is,
    unsigned int flags)
    : boost::archive::text_iarchive_impl<TextIArchiveWithAnySerializer>(is,
                                                                        flags),
      anySerializer(d_) {}
}
