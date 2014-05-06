#include <sfrp/normedvectorspaceutil.hpp>
#include <sfrp/behaviorutil.hpp>
#include <smisc/point1dnormedvectorspace.hpp>

static void compilationTest()
{
  const sfrp::Behavior<double> pos = sfrp::BehaviorUtil::always( 1.0 );
  const sfrp::Behavior<double> velocity = sfrp::BehaviorUtil::always( 1.0 );
  const sfrp::Behavior<double> smoothedPos =
      sfrp::NormedVectorSpaceUtil::smooth(velocity, pos);
}
