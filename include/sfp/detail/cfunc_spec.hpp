#define n BOOST_PP_ITERATION()

#define CFUNC_TP(z, n, data) BOOST_PP_CAT( data, BOOST_PP_ADD(n,1))

template <BOOST_PP_ENUM_PARAMS(n, class T)>
struct cfunc< BOOST_PP_ENUM_PARAMS(n,T)
            , BOOST_PP_ENUM( BOOST_PP_SUB( BOOST_PP_ADD(CFUNC_MAX_SIZE,1) ,n) , CFUNC_PRINT , boost::mpl::void_)
            >
{
    typedef boost::function
            < typename cfunc
                        < BOOST_PP_ENUM( BOOST_PP_SUB( n, 1 ), CFUNC_TP, T )
                        >::type
            ( T0
            )
            > type;
};

#undef CFUNC_TP
#undef n
