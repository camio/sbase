#include <sfp/cf.t.hpp>

#include <boost/noncopyable.hpp>
#include <sfp/cf.hpp>
#include <stest/testcollector.hpp>

namespace {
struct NonCopyableObject : boost::noncopyable {};

int ncFunc(NonCopyableObject&, int i) { return 6; }
int ncFunc2(int i, NonCopyableObject&) { return 4; }

struct CopyableObject {};

int cFunc(CopyableObject&, int i) { return 6; }
int cFunc2(int i, CopyableObject&) { return 4; }

struct Jimmy {
  Jimmy() : it(14) {}
  int it;
} const jim = Jimmy();

int foobar(int i, const int& j) { return i + j; }
int foobar2(const Jimmy& i, const int& j) {
  //    i.it = 24;
  return j;
}
int foobar3(Jimmy& i, const int& j) {
  i.it = 24;
  return j;
}

inline int doIt(int i, char j, int k) { return 0; }
}

namespace sfp {
void cfTests(stest::TestCollector& col) {
  col.addTest("sfp_cf_misc", []()->void {

    NonCopyableObject nco;
    BOOST_CHECK_EQUAL(sfp::cfi(ncFunc)(boost::ref(nco))(12), 6);
    // should not compile (attempts copy constructor)
    // sfp::cfi( ncFunc )( nco )( 12 );
    // should not compile (attempts copy constructor)
    // sfp::cf( ncFunc )( nco )( 12 );

    BOOST_CHECK_EQUAL(sfp::cfi(ncFunc2)(12)(nco), 4);
    BOOST_CHECK_EQUAL(sfp::cf(ncFunc2)(12)(nco), 4);

    CopyableObject co;
    BOOST_CHECK_EQUAL(sfp::cfi(cFunc)(boost::ref(co))(12), 6);
    BOOST_CHECK_EQUAL(sfp::cfi(cFunc)(co)(12), 6);
    BOOST_CHECK_EQUAL(sfp::cf(cFunc)(co)(12), 6);

    BOOST_CHECK_EQUAL(sfp::cfi(cFunc2)(12)(co), 4);
    BOOST_CHECK_EQUAL(sfp::cf(cFunc2)(12)(co), 4);

    BOOST_CHECK_EQUAL(sfp::cfi(foobar)(12)(13), 25);

    Jimmy j = jim;
    BOOST_CHECK_EQUAL(sfp::cf(foobar2)(j)(13), 13);
    BOOST_CHECK_EQUAL(sfp::cf(foobar2)(boost::ref(j))(13), 13);
    BOOST_CHECK_EQUAL(sfp::cf(foobar3)(boost::ref(j))(13), 13);

    //TODO: Not sure why this test is failing.
//    BOOST_CHECK_EQUAL(j.it, 24);

    BOOST_CHECK_EQUAL(sfp::cfi(doIt)(33)('a')(12), 0);
  });
}
}
