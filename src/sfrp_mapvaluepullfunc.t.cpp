#include <sfrp/mapvaluepullfunc.t.hpp>

#include <boost/optional.hpp>
#include <sfrp/mapvaluepullfunc.hpp>
#include <stest/testcollector.hpp>
#include <string>

static void example() {
  // Set 'always3Behavior' to a behavior that always returns 3.
  sfrp::Behavior<int> always3Behavior = sfrp::Behavior<int>::fromValuePullFunc([](
      double time) { return boost::make_optional(3); });

  // Create a 'toStringFunction' that converts 'int's into 'string's.
  auto toStringFunction = [](int i) { return std::to_string(i); };
  typedef decltype(toStringFunction) ToStringFunction;

  // Initialize our 'mapValuePullFunc' object with our function and behavior.
  sfrp::MapValuePullFunc<ToStringFunction, sfrp::Behavior<int>> mapValuePullFunc(
      toStringFunction, always3Behavior);

  // 'result' is set to 'optional<string>("3")'
  boost::optional<std::string> result = mapValuePullFunc(0.0);
}

namespace sfrp {
void mapvaluepullfuncTests(stest::TestCollector& col) {
  col.addTest("sfrp_mapvaluepullfunc_misc", []()->void {
    // Single argument
    Behavior<int> arg0 =
        sfrp::Behavior<int>::fromValuePullFunc([](double time)
                                                   ->boost::optional<int> {
          if (time < 2.0)
            return boost::make_optional(3);
          else
            return boost::none;
        });

    auto toStringFunction = [](int i) { return std::to_string(i); };
    typedef decltype(toStringFunction) ToStringFunction;

    MapValuePullFunc<ToStringFunction, Behavior<int>> mapValuePullFunc(
        toStringFunction, arg0);

    BOOST_CHECK(mapValuePullFunc(0.0) ==
                boost::make_optional(std::string("3")));
    BOOST_CHECK(mapValuePullFunc(3.0) == boost::optional<std::string>());

    // two argument
    Behavior<double> arg1 = sfrp::Behavior<double>::fromValuePullFunc([](
        double time) { return boost::make_optional(time); });

    auto twoArgFunction = [](
        int i, double d) { return std::to_string(i) + std::to_string(int(d)); };
    typedef decltype(twoArgFunction) TwoArgFunction;

    // Also here we're making sure that it accepts reference and const
    // arguments.
    MapValuePullFunc<TwoArgFunction, Behavior<int>&, const Behavior<double>>
        mapValuePullFunc2(twoArgFunction, arg0, arg1);
    BOOST_CHECK(mapValuePullFunc(0.0) ==
                boost::make_optional(std::string("30")));
    BOOST_CHECK(mapValuePullFunc(1.0) ==
                boost::make_optional(std::string("31")));
    BOOST_CHECK(mapValuePullFunc(3.0) == boost::none);
  });
}
}
