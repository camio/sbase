#define n BOOST_PP_FRAME_ITERATION(1)

#define CIOF_FPTR0 BOOST_PP_CAT(T,BOOST_PP_DEC(n)) (*) (BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC(n) , T) )

#define CIOF_FPTR(z) BOOST_PP_CAT(T,BOOST_PP_DEC(n)) (*z) (BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC(n) , T) )

template< BOOST_PP_ENUM_PARAMS(n, typename T) >
typename result< this_type(CIOF_FPTR0)
               >::type
operator()( CIOF_FPTR( f ) ) const
{
    return BOOST_PP_CAT( BOOST_PP_CAT( BOOST_PP_CAT( detail::CurriedIOFunction_, n ), _ ), n )<BOOST_PP_ENUM_PARAMS(n,T) >( f );
}

#undef CIOF_FPTR
#undef CIOF_FPTR0
#undef n
