#include <sfrp/eventutil.t.hpp>

#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <sfrp/behavior.hpp>
#include <sfrp/eventutil.hpp>
#include <stest/testcollector.hpp>

namespace {
struct Color{};
Color white() {
  return Color();
}
struct Drawing{};
struct Point2D{};

static void example1() {
  sfrp::Behavior<boost::optional<Color>> colorChanges;

  sfrp::Behavior<Color> color = sfrp::EventUtil::step( white(), colorChanges );
  sfrp::Behavior<Drawing> drawing;
}
static void example2() {
  sfrp::Behavior<boost::optional<int>> mouseClick;
  sfrp::Behavior<Point2D> mousePos;
  boost::function<Color (Point2D)> posToColor;

  sfrp::Behavior<boost::optional<Color>> colorChanges = sfrp::EventMap()(
      posToColor, sfrp::EventUtil::snapshot_(mousePos, mouseClick));
}
static void example3() {
  sfrp::Behavior<boost::optional<int>> scoreChanges;
  sfrp::Behavior<int> score = sfrp::EventUtil::step(
      0,
      sfrp::EventUtil::accumulate(
          0,
          sfrp::EventMap()([](int change) {
                             return boost::function<int(int)>([change](
                                 int total) { return total + change; });
                           },
                           scoreChanges)));
}
static void example4() {
  sfrp::Behavior<boost::optional<int>> powerupScoreChanges;
  sfrp::Behavior<boost::optional<int>> enemyDestroyedScoreChanges;
  {
    sfrp::Behavior<boost::optional<int>> allScoreChanges =
        sfrp::EventUtil::merge(powerupScoreChanges, enemyDestroyedScoreChanges);
  }
}
}

namespace sfrp {
void eventutilTests(stest::TestCollector& col) {
  col.addTest("sfrp_eventutil_step", []()->void {
    sfrp::Behavior<int> stepped = sfrp::EventUtil::step(
        0,
        sfrp::BehaviorUtil::pure([](double time) {
          return time == 1.0
                     ? boost::make_optional(1)
                     : time == 2.0 ? boost::make_optional(2) : boost::none;
        }));
    BOOST_CHECK_EQUAL(stepped.pull(0.0), boost::make_optional(0));
    BOOST_CHECK_EQUAL(stepped.pull(0.5), boost::make_optional(0));
    BOOST_CHECK_EQUAL(stepped.pull(1.0), boost::make_optional(1));
    BOOST_CHECK_EQUAL(stepped.pull(1.5), boost::make_optional(1));
    BOOST_CHECK_EQUAL(stepped.pull(2.0), boost::make_optional(2));
    BOOST_CHECK_EQUAL(stepped.pull(2.5), boost::make_optional(2));
  });
  col.addTest("sfrp_eventutil_never", []()->void {
    sfrp::Behavior<boost::optional<int>> neverInt =
        sfrp::EventUtil::never<int>();
    BOOST_CHECK_EQUAL(neverInt.pull(0.0),
                      boost::make_optional(boost::optional<int>()));
    BOOST_CHECK_EQUAL(neverInt.pull(1.0),
                      boost::make_optional(boost::optional<int>()));
  });
  col.addTest("sfrp_eventutil_merge", []()->void {
    sfrp::Behavior<boost::optional<int>> lhs =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 0.0
                     ? boost::make_optional(boost::make_optional(3))
                     : time == 1.0
                           ? boost::make_optional(boost::make_optional(3))
                           : boost::make_optional(boost::optional<int>());
        });
    sfrp::Behavior<boost::optional<int>> rhs =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 1.0 ? boost::make_optional(boost::make_optional(9))
                             : boost::make_optional(boost::optional<int>());
        });

    sfrp::Behavior<boost::optional<int>> merged =
        sfrp::EventUtil::merge( lhs, rhs);
    BOOST_CHECK_EQUAL(merged.pull(0.0),
                      boost::make_optional(boost::make_optional(3)));
    BOOST_CHECK_EQUAL(merged.pull(1.0),
                      boost::make_optional(boost::make_optional(3)));
    BOOST_CHECK_EQUAL(merged.pull(1.5),
                      boost::make_optional(boost::optional<int>()));
  });
  col.addTest("sfrp_eventutil_mergeWith", []()->void {
    sfrp::Behavior<boost::optional<int>> lhs =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 0.0
                     ? boost::make_optional(boost::make_optional(3))
                     : time == 1.0
                           ? boost::make_optional(boost::make_optional(3))
                           : boost::make_optional(boost::optional<int>());
        });
    sfrp::Behavior<boost::optional<int>> rhs =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 1.0 ? boost::make_optional(boost::make_optional(9))
                             : boost::make_optional(boost::optional<int>());
        });

    sfrp::Behavior<boost::optional<int>> merged =
        sfrp::EventUtil::mergeWith([](int, int) { return 0; }, lhs, rhs);
    BOOST_CHECK_EQUAL(merged.pull(0.0),
                      boost::make_optional(boost::make_optional(3)));
    BOOST_CHECK_EQUAL(merged.pull(1.0),
                      boost::make_optional(boost::make_optional(0)));
    BOOST_CHECK_EQUAL(merged.pull(1.5),
                      boost::make_optional(boost::optional<int>()));
  });
  col.addTest("sfrp_eventutil_filter", []()->void {
    sfrp::Behavior<boost::optional<int>> event =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 0.0 ? boost::make_optional(boost::make_optional(3))
                             : boost::make_optional(boost::make_optional(4));
        });

    sfrp::Behavior<boost::optional<int>> filtered =
        sfrp::EventUtil::filter([](int i) { return i % 2 == 0; }, event);
    // Check that time 0.0 is filtered out because it is not even.
    BOOST_CHECK_EQUAL(filtered.pull(0.0),
                      boost::make_optional(boost::optional<int>()));
    BOOST_CHECK_EQUAL(filtered.pull(1.0),
                      boost::make_optional(boost::make_optional(4)));
  });
  col.addTest("sfrp_eventutil_accumulate", []()->void {

    // Set 'event' to the event that returns '3' at time '0.0' and '4' at time
    // '1.0'.
    sfrp::Behavior<boost::optional<int>> event =
        sfrp::Behavior<boost::optional<int>>::fromValuePullFunc([](
            double time) {
          return time == 0.0
                     ? boost::make_optional(boost::make_optional(3))
                     : time == 1.0
                           ? boost::make_optional(boost::make_optional(4))
                           : boost::make_optional(boost::optional<int>());
        });

    // Make summer the same as the above, except it adds its argument
    auto summer = sfrp::EventMap()([](int i) -> boost::function<int(int)> {
                                     return [=](int j) { return i + j; };
                                   },
                                   event);

    //Now accumulate summer
    sfrp::Behavior<boost::optional<int>> accumulated =
        sfrp::EventUtil::accumulate(0, summer);

    // Check that time 0.0 is filtered out because it is not even.
    BOOST_CHECK_EQUAL(accumulated.pull(0.0),
                      boost::make_optional(boost::optional<int>(3)));
    BOOST_CHECK_EQUAL(accumulated.pull(0.5),
                      boost::make_optional(boost::optional<int>()));
    BOOST_CHECK_EQUAL(accumulated.pull(1.0),
                      boost::make_optional(boost::optional<int>(7)));
    BOOST_CHECK_EQUAL(accumulated.pull(1.5),
                      boost::make_optional(boost::optional<int>()));
  });
  col.addTest("sfrp_eventutil_snapshot", []()->void {
    sfrp::Behavior<double> timeBehavior = sfrp::BehaviorUtil::time();
    sfrp::Behavior<boost::optional<int>> event =
        sfrp::BehaviorUtil::pure([](double time) {
          return time == 1.0
                     ? boost::make_optional(1)
                     : time == 2.0 ? boost::make_optional(3) : boost::none;
        });
    sfrp::Behavior<boost::optional<std::pair<int, double>>> snapshot =
        sfrp::EventUtil::snapshot(timeBehavior, event);
    BOOST_CHECK(
        snapshot.pull(0.0) ==
        boost::make_optional(boost::optional<std::pair<int, double>>()));
    BOOST_CHECK(snapshot.pull(1.0) == boost::make_optional(boost::make_optional(
                                          std::make_pair(1, 1.0))));
    BOOST_CHECK(
        snapshot.pull(1.5) ==
        boost::make_optional(boost::optional<std::pair<int, double>>()));
    BOOST_CHECK(snapshot.pull(2.0) == boost::make_optional(boost::make_optional(
                                          std::make_pair(3, 2.0))));
  });
  col.addTest("sfrp_eventutil_snapshot2", []()->void {
    sfrp::Behavior<double> timeBehavior = sfrp::BehaviorUtil::time();
    sfrp::Behavior<boost::optional<int>> event =
        sfrp::BehaviorUtil::pure([](double time) {
          return time == 1.0
                     ? boost::make_optional(1)
                     : time == 2.0 ? boost::make_optional(3) : boost::none;
        });
    sfrp::Behavior<boost::optional<double>> snapshot =
        sfrp::EventUtil::snapshot_(timeBehavior, event);
    BOOST_CHECK(snapshot.pull(0.0) ==
                boost::make_optional(boost::optional<double>()));
    BOOST_CHECK(snapshot.pull(1.0) ==
                boost::make_optional(boost::make_optional(1.0)));
    BOOST_CHECK(snapshot.pull(1.5) ==
                boost::make_optional(boost::optional<double>()));
    BOOST_CHECK(snapshot.pull(2.0) ==
                boost::make_optional(boost::make_optional(2.0)));
  });
  col.addTest("sfrp_eventutil_when", []()->void {
    sfrp::Behavior<boost::optional<int>> event =
        sfrp::BehaviorUtil::pure([](double time) {
          return time == 1.0
                     ? boost::make_optional(1)
                     : time == 2.0 ? boost::make_optional(3) : boost::none;
        });
    sfrp::Behavior<bool> whenBehavior = sfrp::BehaviorUtil::pure([](
        double time) { return time < 1.5 ? true : false; });

    sfrp::Behavior<boost::optional<int>> when =
        sfrp::EventUtil::when(whenBehavior, event);

    BOOST_CHECK(when.pull(0.0) == boost::make_optional(boost::optional<int>()));
    BOOST_CHECK(when.pull(1.0) ==
                boost::make_optional(boost::make_optional(1)));
    BOOST_CHECK(when.pull(1.5) == boost::make_optional(boost::optional<int>()));
    BOOST_CHECK(when.pull(2.0) == boost::make_optional(boost::optional<int>()));
  });
  col.addTest("sfrp_eventutil_just", []()->void {
    sfrp::Behavior<boost::optional<boost::optional<int>>> event =
        sfrp::BehaviorUtil::pure([](double time) {
          return time == 1.0
                     ? boost::make_optional(boost::make_optional(1))
                     : time == 2.0
                           ? boost::make_optional(boost::optional<int>())
                           : boost::none;
        });
    sfrp::Behavior<boost::optional<int>> justedEvent = sfrp::EventUtil::just( event );

    BOOST_CHECK(justedEvent.pull(0.0) == boost::make_optional(boost::optional<int>()));
    BOOST_CHECK(justedEvent.pull(1.0) ==
                boost::make_optional(boost::make_optional(1)));
    BOOST_CHECK(justedEvent.pull(1.5) == boost::make_optional(boost::optional<int>()));
    BOOST_CHECK(justedEvent.pull(2.0) == boost::make_optional(boost::optional<int>()));
  });
}
}
