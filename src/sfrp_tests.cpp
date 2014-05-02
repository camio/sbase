#include <sfrp/tests.hpp>

#include <sfrp/behavior.t.hpp>
#include <sfrp/behaviormap.t.hpp>
#include <sfrp/behaviorpuller.t.hpp>
#include <sfrp/cachedincreasingpartialtimefunction.t.hpp>
#include <sfrp/increasingpartialtimefunction.t.hpp>
#include <sfrp/util.t.hpp>
#include <sfrp/vectorspaceutil.t.hpp>
#include <sfrp/wormhole.t.hpp>

namespace sfrp {
void tests( stest::TestCollector & col ) {
  behaviorTests( col );
  behaviormapTests( col );
  behaviorpullerTests( col );
  cachedincreasingpartialtimefunctionTests( col );
  increasingpartialtimefunctionTests( col );
  utilTests( col );
  vectorspaceutilTests( col );
  wormholeTests( col );
}
}
