#include <scpp/tests.hpp>

#include <scpp/setutil.t.hpp>
#include <scpp/vectorutil.t.hpp>

namespace scpp {
void tests(stest::TestCollector& col)
{
  setutilTests( col );
  vectorutilTests( col );
}
}
