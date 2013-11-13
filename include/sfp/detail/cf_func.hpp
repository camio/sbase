#define n BOOST_PP_FRAME_ITERATION(1)

#define CF_FPTR0 BOOST_PP_CAT(T,BOOST_PP_DEC(n)) (*) (BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC(n) , T) )

#define CF_FPTR(z) BOOST_PP_CAT(T,BOOST_PP_DEC(n)) (*z) (BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC(n) , T) )

template< BOOST_PP_ENUM_PARAMS(n, typename T) >
typename result< this_type(CF_FPTR0)
               >::type
operator()( CF_FPTR( f ) ) const
{
    return BOOST_PP_CAT( BOOST_PP_CAT( BOOST_PP_CAT( detail::CurriedFunction_, n ), _ ), n )<BOOST_PP_ENUM_PARAMS(n,T) >( f );
}

#undef CF_FPTR
#undef CF_FPTR0
#undef n
