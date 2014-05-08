#include <sfrp/tests.hpp>

#include <sfrp/behavior.t.hpp>
#include <sfrp/behaviordebugutil.t.hpp>
#include <sfrp/behaviormap.t.hpp>
#include <sfrp/behavioroperators.t.hpp>
#include <sfrp/behaviorpairutil.t.hpp>
#include <sfrp/behaviorpuller.t.hpp>
#include <sfrp/behaviortimeutil.t.hpp>
#include <sfrp/behaviorutil.t.hpp>
#include <sfrp/eventdebugutil.t.hpp>
#include <sfrp/eventmap.t.hpp>
#include <sfrp/eventmapfunctionadapter.t.hpp>
#include <sfrp/eventutil.t.hpp>
#include <sfrp/cachedincreasingpartialtimefunction.t.hpp>
#include <sfrp/increasingpartialtimefunction.t.hpp>
#include <sfrp/vectorspaceutil.t.hpp>
#include <sfrp/wormhole.t.hpp>

namespace sfrp {
void tests( stest::TestCollector & col ) {
  behaviorTests( col );
  behaviordebugutilTests( col );
  behaviormapTests( col );
  behavioroperatorsTests( col );
  behaviorpairutilTests( col );
  behaviorpullerTests( col );
  behaviortimeutilTests( col );
  behaviorutilTests( col );
  eventdebugutilTests( col );
  eventmapTests( col );
  eventmapfunctionadapterTests( col );
  eventutilTests( col );
  cachedincreasingpartialtimefunctionTests( col );
  increasingpartialtimefunctionTests( col );
  vectorspaceutilTests( col );
  wormholeTests( col );
}
}
