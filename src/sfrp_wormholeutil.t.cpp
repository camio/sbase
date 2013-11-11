#include <sfrp/wormhole.hpp>
#include <sfrp/wormholeutil.hpp>

static void compilationTest()
{
  sfrp::Wormhole<int> w = sfrp::WormholeUtil::createWormhole( 33 );
}
