#include <scpp/tests.hpp>

#include <scpp/pairutil.t.hpp>
#include <scpp/setutil.t.hpp>
#include <scpp/vectorutil.t.hpp>

namespace scpp {
void tests(stest::TestCollector& col)
{
  pairutilTests( col );
  setutilTests( col );
  vectorutilTests( col );
}
}
