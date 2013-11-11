#include <sfrp/wormhole.t.hpp>

#include <sfrp/util.hpp> // +, pmConst
#include <sfrp/wormhole.hpp>
#include <stest/testcollector.hpp>

namespace sfrp{
void wormholeTests(stest::TestCollector& col)
{
  col.addTest("sfrp_wormhole_misc", []()->void {
    const sfrp::Wormhole<int> w(12);

    BOOST_CHECK( ! w.closed() );

    const auto b = w.outputBehavior() + sfrp::pmConst(1);

    const auto b2 = w.setInputBehavior( b );
    BOOST_CHECK( w.closed() );

    BOOST_CHECK_EQUAL( *b2.pull( 0.0 ), 13 );
    BOOST_CHECK_EQUAL( *b2.pull( 1.0 ), 14 );
    BOOST_CHECK_EQUAL( *b2.pull( 2.0 ), 15 );
  });
}
}
