#define n BOOST_PP_ITERATION()

#define CIOFUNC_TP(z, n, data) BOOST_PP_CAT( data, BOOST_PP_ADD(n,1))

template <BOOST_PP_ENUM_PARAMS(n, class T)>
struct ciofunc< BOOST_PP_ENUM_PARAMS(n,T)
            , BOOST_PP_ENUM( BOOST_PP_SUB( BOOST_PP_ADD(CIOFUNC_MAX_SIZE,1) ,n) , CIOFUNC_PRINT , boost::mpl::void_)
            >
{
    typedef boost::function
            < typename ciofunc
                        < BOOST_PP_ENUM( BOOST_PP_SUB( n, 1 ), CIOFUNC_TP, T )
                        >::type
            ( T0
            )
            > type;
};

#undef CIOFUNC_TP
#undef n
