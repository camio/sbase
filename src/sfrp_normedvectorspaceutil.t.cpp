#include <sfrp/normedvectorspaceutil.t.hpp>

#include <boost/optional/optional_io.hpp>
#include <sfrp/normedvectorspaceutil.hpp>
#include <sfrp/behaviorutil.hpp>
#include <smisc/point1dnormedvectorspace.hpp>
#include <smisc/point2d.hpp>
#include <smisc/point2dnormedvectorspace.hpp>
#include <stest/testcollector.hpp>

static sfrp::Behavior<smisc::Point2D> missileFollow(
    smisc::Point2D initialMissilePosition,
    sfrp::Behavior<smisc::Point2D> targetPosition) {
  const sfrp::Behavior<smisc::Point1D> missileVelocity =
      sfrp::BehaviorUtil::always(3.0);
  return sfrp::NormedVectorSpaceUtil::smooth(
      missileVelocity,
      sfrp::BehaviorTimeUtil::replaceInitialValue(targetPosition,
                                                  initialMissilePosition));
}

namespace sfrp {
void normedvectorspaceutilTests(stest::TestCollector& col) {
  col.addTest("sfrp_normedvectorspaceutil_smooth", []()->void {
    {
      sfrp::Behavior<double> target = sfrp::BehaviorUtil::pure([](double time) {
        return time == 0.0 ? 10.0 : 20.0;
      });
      sfrp::Behavior<double> velocity = sfrp::BehaviorUtil::always(1.0);
      sfrp::Behavior<double> smoothed =
          sfrp::NormedVectorSpaceUtil::smooth(velocity, target);

      BOOST_CHECK_EQUAL(smoothed.pull(0.0), boost::make_optional(10.0));
      BOOST_CHECK_EQUAL(smoothed.pull(1.0), boost::make_optional(11.0));
      BOOST_CHECK_EQUAL(smoothed.pull(10.0), boost::make_optional(20.0));
      BOOST_CHECK_EQUAL(smoothed.pull(20.0), boost::make_optional(20.0));
    }
    // Check that the initial 'pull' is always the same as the initial pull on
    // the target.
    {
      sfrp::Behavior<double> target = sfrp::BehaviorUtil::pure([](double time) {
        return time <= 5.0 ? 10.0 : 20.0;
      });
      sfrp::Behavior<double> velocity = sfrp::BehaviorUtil::always(1.0);
      sfrp::Behavior<double> smoothed =
          sfrp::NormedVectorSpaceUtil::smooth(velocity, target);

      BOOST_CHECK_EQUAL(smoothed.pull(5.0), boost::make_optional(10.0));
      BOOST_CHECK_EQUAL(smoothed.pull(6.0), boost::make_optional(11.0));
      BOOST_CHECK_EQUAL(smoothed.pull(15.0), boost::make_optional(20.0));
      BOOST_CHECK_EQUAL(smoothed.pull(25.0), boost::make_optional(20.0));
    }
  });
}
}
