#define m BOOST_PP_ITERATION()

#define CurriedFunction_n_m BOOST_PP_CAT( BOOST_PP_CAT( BOOST_PP_CAT( CurriedFunction_, n ), _ ), m )

#define CurriedFunction_n_prevm \
    BOOST_PP_CAT( BOOST_PP_CAT( BOOST_PP_CAT( CurriedFunction_\
                                            , n )\
                              , _\
                              )\
                , BOOST_PP_DEC( m )\
                )

#define CF_DEFINST(z, n, data) BOOST_PP_CAT( a, n )( BOOST_PP_CAT( BOOST_PP_CAT( a, n ), _ ) )

#define CF_ARG(z, n, data) const BOOST_PP_CAT( V, n ) & BOOST_PP_CAT( BOOST_PP_CAT( a, n ), _ )

#define CF_DECL(z, n, data) mutable BOOST_PP_CAT( V, n ) BOOST_PP_CAT( a, n );

#define CF_RETVAL BOOST_PP_IF( BOOST_PP_EQUAL( m, 2 )\
                           , f( BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC( n ), a) )\
                           , R( f\
                              , BOOST_PP_ENUM_PARAMS(\
                                  BOOST_PP_INC( BOOST_PP_SUB( n, m ))\
                                                    , a)\
                              )\
                           )
#define CF_ARGS_TYPE BOOST_PP_CAT( T, BOOST_PP_SUB( n, m ) )
#define CF_ARGS CF_ARGS_TYPE BOOST_PP_CAT( a, BOOST_PP_SUB( n, m ) )

#define CF_ARGS_TYPE_SANE typename sanitize<CF_ARGS_TYPE>::type
#define CF_ARGS_REF const boost::reference_wrapper<CF_ARGS_TYPE_SANE> & BOOST_PP_CAT( a, BOOST_PP_SUB( n, m ) )

#define CF_TEMPLATE_ARGS BOOST_PP_ENUM_PARAMS(n, typename T)\
 BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_SUB( n, m ), typename V )

#define CF_TEMPLATE_ARGS_NT BOOST_PP_ENUM_PARAMS(n, T)\
 BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_SUB( n, m ), V )

#define end() typedef BOOST_PP_CAT( T, BOOST_PP_DEC( n )) result_type;
#define endret() result_type
#define prevm() \
    template< typename T >\
    struct result;\
    template< typename Y >\
    struct result<this_type( Y )>\
    {\
      typedef typename sanitize<Y>::type Z;\
      typedef CurriedFunction_n_prevm<CF_TEMPLATE_ARGS_NT, Z > type;\
    };

#define prevmret() typename result<this_type(CF_ARGS_TYPE)>::type
#define prevmret_ref() typename result<this_type(boost::reference_wrapper<CF_ARGS_TYPE_SANE>)>::type
#define refhandle()\
        prevmret_ref() \
        operator()( CF_ARGS_REF ) const\
        {\
            typedef prevmret_ref() R;\
            return CF_RETVAL;\
        }

namespace detail
{
    template< CF_TEMPLATE_ARGS >
    struct CurriedFunction_n_m
    {
        typedef CurriedFunction_n_m<CF_TEMPLATE_ARGS_NT> this_type;
        BOOST_PP_IF( BOOST_PP_EQUAL( m, 2 ), end, prevm )()

        CurriedFunction_n_m
            ( CF_FPTR( f_ )
            BOOST_PP_ENUM_TRAILING( BOOST_PP_SUB( n, m ), CF_ARG, Z )
            )
            : f( f_ )
            BOOST_PP_ENUM_TRAILING( BOOST_PP_SUB( n, m ), CF_DEFINST, Z )
        {
        }
        CF_FPTR( f );
        BOOST_PP_REPEAT( BOOST_PP_SUB( n, m ), CF_DECL, Z )

        BOOST_PP_IF( BOOST_PP_EQUAL( m, 2 ), endret, prevmret )()
        operator()( CF_ARGS ) const
        {
            typedef BOOST_PP_IF( BOOST_PP_EQUAL( m, 2 ), endret, prevmret )() R;
            return CF_RETVAL;
        }
        BOOST_PP_IF( BOOST_PP_EQUAL( m, 2 ), BOOST_PP_EMPTY, refhandle )()
    };
}
#undef m
#undef CurriedFunction_n_m
#undef CurriedFunction_n_prevm
#undef CF_DEFINST
#undef CF_ARG
#undef CF_DECL
#undef CF_RETVAL
#undef CF_ARGS_TYPE
#undef CF_ARGS
#undef CF_ARGS_TYPE_SANE
#undef CF_ARGS_REF
#undef CF_TEMPLATE_ARGS
#undef CF_TEMPLATE_ARGS_NT
#undef end
#undef endret
#undef prevm
#undef prevmret
#undef prevmret_ref
#undef refhandle
