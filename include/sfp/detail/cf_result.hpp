#define n BOOST_PP_FRAME_ITERATION(1)

#define CF_FPTR0 BOOST_PP_CAT(T,BOOST_PP_DEC(n)) (* )(BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC(n) , T) )

template< BOOST_PP_ENUM_PARAMS(n, typename T) >
struct result< this_type( CF_FPTR0 ) >
{
    typedef typename cfunc<BOOST_PP_ENUM_PARAMS(n,T)>::type type;
};

#undef CF_FPTR0
#undef n
