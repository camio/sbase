#include <sboost/optionalutil.t.hpp>

#include <sboost/optionalutil.hpp>
#include <stest/testcollector.hpp>
#include <iostream>
#include <string>

// Dummy implementation used in the example below.
static boost::optional<std::string> readString()
{
  return boost::none;
}

static void example() {
  const boost::optional<std::string> combinedStrings = sboost::OptionalUtil::bind(
      readString(), [](const std::string & firstStringRead) {
        return sboost::OptionalUtil::bind(
            readString(),
            [firstStringRead](const std::string & secondStringRead) {
              return boost::make_optional(firstStringRead + secondStringRead);
            });
      });
  if (!combinedStrings)
    std::cout << "There was an error reading at least one of the strings!"
              << std::endl;
  else
    std::cout << "Your strings concatenated are: " << *combinedStrings
              << std::endl;
}

static std::string f(int) { return "foo"; }

namespace sboost {
void optionalutilTests(stest::TestCollector& col) {
  col.addTest("sboost_optionalutil_join", []()->void {
    BOOST_CHECK(OptionalUtil_Join()(boost::make_optional(
                    boost::make_optional(3))) == boost::make_optional(3));
    BOOST_CHECK(OptionalUtil_Join()(boost::optional<boost::optional<int>>()) ==
                boost::none);
  });
  col.addTest("sboost_optionalutil_alternative", []()->void {
    BOOST_CHECK(OptionalUtil::alternative(boost::make_optional(3),
                                          boost::make_optional(4)) ==
                boost::make_optional(3));
    BOOST_CHECK(OptionalUtil::alternative(boost::optional<int>(),
                                          boost::make_optional(4)) ==
                boost::make_optional(4));
  });
  col.addTest("sboost_optionalutil_GetValueOr", []()->void {
    BOOST_CHECK_EQUAL(OptionalUtil_GetValueOr()(boost::make_optional(3), 6), 3);
    BOOST_CHECK_EQUAL(OptionalUtil_GetValueOr()(boost::optional<int>(), 6), 6);
    BOOST_CHECK_EQUAL(OptionalUtil::getValueOr(boost::make_optional(3), 6), 3);
  });
  col.addTest("sboost_optionalutil_map", []()->void {
    // Check anonymous functions
    BOOST_CHECK(
        OptionalUtil::map([](int i)->std::string { return std::to_string(i); },
                          boost::make_optional(5)) ==
        boost::make_optional(std::string("5")));
    BOOST_CHECK(
        OptionalUtil::map([](int i)->std::string { return std::to_string(i); },
                          boost::optional<int>()) == boost::none);

    // Check function pointer
    BOOST_CHECK(OptionalUtil::map(f, boost::make_optional(5)) ==
                boost::make_optional(std::string("foo")));
    BOOST_CHECK(OptionalUtil::map(f, boost::optional<int>()) == boost::none);
  });
  col.addTest("sboost_optionalutil_bind", []()->void {
    BOOST_CHECK(OptionalUtil::bind(boost::make_optional(3),
                                       [](int i)->boost::optional<std::string> {
                  if (i == 0)
                    return boost::none;
                  else
                    return std::to_string(i);
                }) == boost::make_optional(std::string("3")));
    BOOST_CHECK(OptionalUtil::bind(boost::make_optional(0),
                                       [](int i)->boost::optional<std::string> {
                  if (i == 0)
                    return boost::none;
                  else
                    return std::to_string(i);
                }) == boost::none);
    BOOST_CHECK(OptionalUtil::bind(boost::optional<int>(),
                                       [](int i)->boost::optional<std::string> {
                  if (i == 0)
                    return boost::none;
                  else
                    return std::to_string(i);
                }) == boost::none);
  });
}
}
