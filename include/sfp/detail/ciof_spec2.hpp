#define m BOOST_PP_ITERATION()

#define CurriedIOFunction_n_m BOOST_PP_CAT( BOOST_PP_CAT( BOOST_PP_CAT( CurriedIOFunction_, n ), _ ), m )

#define CurriedIOFunction_n_prevm BOOST_PP_CAT( BOOST_PP_CAT( BOOST_PP_CAT( CurriedIOFunction_, n ), _ ), BOOST_PP_DEC( m)  )

#define CIOF_DEFINST(z, n, data) BOOST_PP_CAT( a, n )( BOOST_PP_CAT( BOOST_PP_CAT( a, n ), _ ) )

#define CIOF_ARG(z, n, data) const BOOST_PP_CAT( V, n ) & BOOST_PP_CAT( BOOST_PP_CAT( a, n ), _ )

#define CIOF_ARGS_TYPE BOOST_PP_CAT( T, BOOST_PP_SUB( n, m ) )
#define CIOF_ARGS_TYPE_SANE typename sanitize<CIOF_ARGS_TYPE>::type
#define CIOF_ARGS_REF const boost::reference_wrapper<CIOF_ARGS_TYPE_SANE> & BOOST_PP_CAT( a, BOOST_PP_SUB( n, m ) )
#define CIOF_ARGS BOOST_PP_IF( BOOST_PP_EQUAL( m, 1 )\
                         , BOOST_PP_EMPTY()\
                         , CIOF_ARGS_TYPE \
                           BOOST_PP_CAT( a, BOOST_PP_SUB( n, m ) )\
                         )

#define CIOF_DECL(z, n, data) mutable BOOST_PP_CAT( V, n ) BOOST_PP_CAT( a, n );

#define end() typedef BOOST_PP_CAT( T, BOOST_PP_DEC( n )) result_type;
#define endret() result_type
#define prevm() \
    template< typename T >\
    struct result;\
    template< typename Y >\
    struct result<this_type( Y )>\
    {\
      typedef typename sanitize<Y>::type Z;\
      typedef CurriedIOFunction_n_prevm<CIOF_TEMPLATE_ARGS_NT, Z > type;\
    };
#define prevmret() typename result<this_type(CIOF_ARGS_TYPE)>::type

#define prevmret_ref() typename result<this_type(boost::reference_wrapper<CIOF_ARGS_TYPE_SANE>)>::type

#define refhandle()\
        prevmret_ref() \
        operator()( CIOF_ARGS_REF ) const\
        {\
            typedef prevmret_ref() R;\
            return CIOF_RETVAL;\
        }

#define CIOF_TEMPLATE_ARGS BOOST_PP_ENUM_PARAMS(n, typename T)\
 BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_SUB( n, m ), typename V )

#define CIOF_TEMPLATE_ARGS_NT BOOST_PP_ENUM_PARAMS(n, T)\
 BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_SUB( n, m ), V )

#define CIOF_RETVAL\
                BOOST_PP_IF( BOOST_PP_EQUAL( m, 1 )\
                           , f( BOOST_PP_ENUM_PARAMS( BOOST_PP_DEC( n ), a) )\
                           , R( f\
                              , BOOST_PP_ENUM_PARAMS(\
                                            BOOST_PP_INC( BOOST_PP_SUB( n, m ))\
                                            , a)\
                              )\
                           )

namespace detail
{
    template< CIOF_TEMPLATE_ARGS >
    struct CurriedIOFunction_n_m
    {
        typedef CurriedIOFunction_n_m<CIOF_TEMPLATE_ARGS_NT> this_type;
        BOOST_PP_IF( BOOST_PP_EQUAL( m, 1 ), end, prevm )()

        CurriedIOFunction_n_m
            ( CIOF_FPTR( f_ )
              BOOST_PP_ENUM_TRAILING( BOOST_PP_SUB( n, m ), CIOF_ARG, Z )
            )
            : f( f_ )
            BOOST_PP_ENUM_TRAILING( BOOST_PP_SUB( n, m ), CIOF_DEFINST, Z )
        {
        }
        CIOF_FPTR( f );
        BOOST_PP_REPEAT( BOOST_PP_SUB( n, m ), CIOF_DECL, Z )

        BOOST_PP_IF( BOOST_PP_EQUAL( m, 1 ), endret, prevmret )()
        operator()
            ( CIOF_ARGS
            ) const
        {
            typedef BOOST_PP_IF( BOOST_PP_EQUAL( m, 1 ), endret, prevmret )() R;
            return CIOF_RETVAL;
        }
        BOOST_PP_IF( BOOST_PP_EQUAL( m, 1 ), BOOST_PP_EMPTY, refhandle )()
    };
}
#undef m
#undef CurriedIOFunction_n_m
#undef CurriedIOFunction_n_prevm
#undef CIOF_DEFINST
#undef CIOF_ARG
#undef CIOF_ARGS_TYPE
#undef CIOF_ARGS_TYPE_SANE
#undef CIOF_ARGS_REF
#undef CIOF_ARGS
#undef CIOF_DECL
#undef end
#undef endret
#undef prevm
#undef prevmret
#undef prevmret_ref
#undef refhandle
#undef CIOF_TEMPLATE_ARGS
#undef CIOF_TEMPLATE_ARGS_NT
#undef CIOF_RETVAL
