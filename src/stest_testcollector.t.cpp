#include <stest/testcollector.hpp>

// The following code in the example namespace is there primarily to ensure
// that the example in the documentation for testcollector compiles okay.

namespace example {
template <typename T>
class Optional {
 public:
  Optional();
  Optional(T);
  bool operator==(Optional<T>) const;
  bool operator!=(Optional<T>) const;
  bool hasValue() const;
  // The behavior is undefined unless 'hasValue() == true'.
  T getValue() const;

 private:
  bool m_hasValue;
  T m_value;
};

void optionalTests(stest::TestCollector&);
}

namespace example {
void optionalTests(stest::TestCollector& col) {
  col.addTest("example_optional_equality", []()->void {
    BOOST_CHECK(Optional<int>(12) == Optional<int>(12));
    BOOST_CHECK(Optional<int>(12) != Optional<int>(14));
    BOOST_CHECK(Optional<int>(12) != Optional<int>());
  });
  col.addTest("example_optional_getValue", []()->void {
    BOOST_CHECK(Optional<int>(12).hasValue());
    BOOST_CHECK(Optional<int>(12).getValue() == 12);
    BOOST_CHECK(!Optional<int>().hasValue());
  });
}
}
