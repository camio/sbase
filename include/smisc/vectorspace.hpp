#ifndef SMISC_VECTORSPACE_HPP_
#define SMISC_VECTORSPACE_HPP_

namespace smisc {

/** This is a typeclass for vector spaces.
 *  This zero element should work as the additive identity.
 *  You are asserting that += works and multiplication by scalars works
 *  with '*'.
 */
template <typename T>
struct VectorSpace;

template <typename T>
T zero();

// ===========================================================================
//                 INLINE DEFINITIONS
// ===========================================================================

template <typename T>
T zero() {
  return VectorSpace<T>().zero_();
}

}
#endif
