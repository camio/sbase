#define n BOOST_PP_FRAME_ITERATION(1)

#define CIOF_FPTR(z) BOOST_PP_CAT(T,BOOST_PP_DEC(n)) (* z )(BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC(n) , T) )

#define BOOST_PP_ITERATION_LIMITS (1, n)
#define BOOST_PP_FILENAME_2 "sfp/detail/ciof_spec2.hpp"
#include BOOST_PP_ITERATE()

#undef CIOF_FPTR
#undef n
