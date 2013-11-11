#include <sfp/ciof.t.hpp>

#include <boost/noncopyable.hpp>
#include <sfp/ciof.hpp>
#include <stest/testcollector.hpp>

namespace {
struct NonCopyableObject : boost::noncopyable {};

int ncFunc(NonCopyableObject&, int i) { return 6; }
int ncFunc2(int i, NonCopyableObject&) { return 4; }

struct CopyableObject {};

int cFunc(CopyableObject&, int i) { return 6; }
int cFunc2(int i, CopyableObject&) { return 4; }
}

namespace sfp {
void ciofTests(stest::TestCollector& col) {
  col.addTest("sfp_ciof_misc", []()->void {

    NonCopyableObject nco;

    BOOST_CHECK_EQUAL(sfp::ciofi(ncFunc)(boost::ref(nco))(12)(), 6);

    // should not compile
    // sfp::ciofi( ncFunc )( nco )( 12 )();

    // should not compile
    // sfp::ciof( ncFunc )( nco )( 12 )();

    BOOST_CHECK_EQUAL(sfp::ciofi(ncFunc2)(12)(boost::ref(nco))(), 4);

    // should not compile
    // sfp::ciofi( ncFunc2 )( 12 )( nco )();

    // should not compile
    // sfp::ciof( ncFunc2 )( 12 )( nco )();

    CopyableObject co;

    BOOST_CHECK_EQUAL(sfp::ciofi(cFunc)(boost::ref(co))(12)(), 6);
    BOOST_CHECK_EQUAL(sfp::ciofi(cFunc)(co)(12)(), 6);
    BOOST_CHECK_EQUAL(sfp::ciof(cFunc)(co)(12)(), 6);

    BOOST_CHECK_EQUAL(sfp::ciofi(cFunc2)(12)(boost::ref(co))(), 4);
    BOOST_CHECK_EQUAL(sfp::ciofi(cFunc2)(12)(co)(), 4);
    BOOST_CHECK_EQUAL(sfp::ciof(cFunc2)(12)(co)(), 4);
  });
}
}
