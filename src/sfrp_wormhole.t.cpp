#include <sfrp/wormhole.t.hpp>

#include <sfrp/behaviorutil.hpp>
#include <sfrp/behavioroperators.hpp> // +
#include <sfrp/wormhole.hpp>
#include <stest/testcollector.hpp>

template <typename T>
static sfrp::Behavior<std::pair<T, T>> pmWithPrev(const T& initialValue,
                                                  const sfrp::Behavior<T>& b)
{
  Wormhole<T> w = Wormhole<T>(initialValue);
  return pmZip(w.outputBehavior(), w.setInputBehavior(b));
}

namespace sfrp{
void wormholeTests(stest::TestCollector& col)
{
  col.addTest("sfrp_wormhole_misc", []()->void {
    const sfrp::Wormhole<int> w(12);

    BOOST_CHECK( ! w.closed() );

    const auto b = w.outputBehavior() + sfrp::BehaviorUtil::always(1);

    const auto b2 = w.setInputBehavior( b );
    BOOST_CHECK( w.closed() );

    BOOST_CHECK_EQUAL( *b2.pull( 0.0 ), 13 );
    BOOST_CHECK_EQUAL( *b2.pull( 1.0 ), 14 );
    BOOST_CHECK_EQUAL( *b2.pull( 2.0 ), 15 );
  });
}
}
