#include <stest/serialization.hpp>

#include <boost/optional.hpp>
#include <boost/serialization/optional.hpp>
#include <stest/testcollector.hpp>

namespace stest {
    class BadSerialization
    {
    public:
        BadSerialization( )
            : member0( 99 )
            , member1( 28 )
        {
        }
        BadSerialization( int member0_, int member1_ )
            : member0( member0_ )
            , member1( member1_ )
        {
        }

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar & boost::serialization::make_nvp( "member0", member0 );
            //Note, that member1 is not included here.
        }

        bool operator==( const BadSerialization & b ) const
        {
            return member0 == b.member0
                && member1 == b.member1;
        }

        int member0;
        int member1;
    };

    void serializationTests( stest::TestCollector & col )
    {
        col.addTest
            ( "stest_serialization"
            , []()
              {
                  // Just a sanity check that our round trip test works for
                  // correctly defined types
                  BOOST_CHECK( stest::serializableRoundtrip( boost::optional<int>( 4 ) ) );
                  BOOST_CHECK( stest::serializableRoundtrip( boost::optional<int>( ) ) );

                  // This check should pass if serializableHasConsistentRoundtrip
                  // correctly detects that BadSerialization has an inconsistant
                  // round-trip.
                  BOOST_CHECK( !stest::serializableRoundtrip( BadSerialization( 12, 24 )));
              }
            );
    }
}
