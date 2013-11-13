#include <sfrp/normedvectorspaceutil.hpp>
#include <sfrp/util.hpp>
#include <smisc/point1dnormedvectorspace.hpp>

static void compilationTest()
{
  const sfrp::Behavior<double> pos = sfrp::pmConst( 1.0 );
  const sfrp::Behavior<double> velocity = sfrp::pmConst( 1.0 );
  const sfrp::Behavior<double> smoothedPos =
      sfrp::NormedVectorSpaceUtil::smooth(velocity, pos);
}
