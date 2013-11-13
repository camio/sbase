#define n BOOST_PP_FRAME_ITERATION(1)

#define CF_FPTR(z) BOOST_PP_CAT(T,BOOST_PP_DEC(n)) (* z )(BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC(n) , T) )

#define BOOST_PP_ITERATION_LIMITS (2, n)
#define BOOST_PP_FILENAME_2 "sfp/detail/cf_spec2.hpp"
#include BOOST_PP_ITERATE()

#undef CF_FPTR
#undef n
