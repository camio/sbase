#include <sfrp/wormhole.hpp>
#include <sfrp/wormholeutil.hpp>

static void example() {
  auto doubleWormhole = sfrp::WormholeUtil::createWormhole( 0.0 );
  auto doubleWormhole2 = sfrp::Wormhole<double>( 0.0 );
}
static void compilationTest()
{
  sfrp::Wormhole<int> w = sfrp::WormholeUtil::createWormhole( 33 );
}
