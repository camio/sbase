#include <sfrp/tests.hpp>

#include <sfrp/util.t.hpp>
#include <sfrp/vectorspaceutil.t.hpp>
#include <sfrp/wormhole.t.hpp>

namespace sfrp {
void tests( stest::TestCollector & col ) {
  utilTests( col );
  vectorspaceutilTests( col );
  wormholeTests( col );
}
}
