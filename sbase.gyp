{
    'includes': [
        '../build/common.gypi',
    ],

    'targets': [
        {
            "target_name": 'sbasetest_main',
            "type": 'executable',
            'dependencies': [
                '../boost-gyp/boost.gyp:boost.test',
                'sbasetest',
                'stest',
            ],
            'sources': [
                'src/sbasetest_main.cpp',
            ],
        },
        {
            "target_name": 'sbasetest',
            "type": 'static_library',
            'dependencies': [
                'scpp',
                'sboost',
                'sfp',
                'sfrp',
                'smisc',
                'stest',
            ],
            'direct_dependent_settings': {
                'include_dirs': [ 'include' ],
            },
            'include_dirs': [ 'include', ],
            'sources': [
                'src/sbasetest_tests.cpp',
            ],
        },
        {
            "target_name": 'sboost',
            "type": 'static_library',
            'dependencies': [
                '../boost-gyp/boost.gyp:boost.headers',
                '../boost-gyp/boost.gyp:boost.serialization',
                'stest'
            ],
            'export_dependent_settings': [
                '../boost-gyp/boost.gyp:boost.headers',
                '../boost-gyp/boost.gyp:boost.serialization',
            ],
            'include_dirs': [ 'include', ],
            'sources': [
                'src/sboost_fusionatc.cpp',
                'src/sboost_fusionfoldr.cpp',
                'src/sboost_fusionfoldr.t.cpp',
                'src/sboost_optional.cpp',
                'src/sboost_serialize.cpp',
                'src/sboost_tests.cpp',
                'src/sboost_variant.cpp',
            ],
        },
        {
            "target_name": 'scpp',
            "type": 'static_library',
            'dependencies': [
                '../boost-gyp/boost.gyp:boost.headers',
                'stest',
            ],
            'export_dependent_settings': [
                '../boost-gyp/boost.gyp:boost.headers',
            ],
            'include_dirs': [ 'include' ],
            'sources': [
                'src/scpp_operators.cpp',
                'src/scpp_setutil.cpp',
                'src/scpp_setutil.t.cpp',
                'src/scpp_string.cpp',
                'src/scpp_tests.cpp',
                'src/scpp_utility.cpp',
                'src/scpp_vectorutil.cpp',
                'src/scpp_vectorutil.t.cpp',
            ],
        },
        {
            "target_name": 'sfp',
            "type": 'static_library',
            'dependencies': [
                '../boost-gyp/boost.gyp:boost.headers',
                'stest',
            ],
            'export_dependent_settings': [
                '../boost-gyp/boost.gyp:boost.headers',
            ],
            'include_dirs': [ 'include' ],
            'sources': [
                'src/sfp_cf.cpp',
                'src/sfp_cf.t.cpp',
                'src/sfp_ciof.cpp',
                'src/sfp_ciof.t.cpp',
                'src/sfp_flip.cpp',
                'src/sfp_tests.cpp',
            ],
        },
        {
            "target_name": 'sfrp',
            "type": 'static_library',
            'dependencies': [
                '../boost-gyp/boost.gyp:boost.headers',
                'sboost',
                'scpp',
                'sfp',
                'smisc',
                'stest',
            ],
            'export_dependent_settings': [
                '../boost-gyp/boost.gyp:boost.headers',
                'sboost',
                'scpp',
                'sfp',
                'smisc',
            ],
            'include_dirs': [ 'include' ],
            'sources': [
                'src/sfrp_behavior.cpp',
                'src/sfrp_normedvectorspaceutil.cpp',
                'src/sfrp_normedvectorspaceutil.t.cpp',
                'src/sfrp_tests.cpp',
                'src/sfrp_util.cpp',
                'src/sfrp_util.t.cpp',
                'src/sfrp_vectorspaceutil.cpp',
                'src/sfrp_vectorspaceutil.t.cpp',
                'src/sfrp_wormhole.cpp',
                'src/sfrp_wormhole.t.cpp',
                'src/sfrp_wormholeutil.cpp',
                'src/sfrp_wormholeutil.t.cpp',
            ],
        },
        {
            "target_name": 'smisc',
            "type": 'static_library',
            'dependencies': [
                '../boost-gyp/boost.gyp:boost.headers',
                '../boost-gyp/boost.gyp:boost.thread',
                '../boost-gyp/boost.gyp:boost.serialization',
                'sboost',
                'stest',
            ],
            'export_dependent_settings': [
                '../boost-gyp/boost.gyp:boost.headers',
                '../boost-gyp/boost.gyp:boost.thread',
                '../boost-gyp/boost.gyp:boost.serialization',
                'sboost',
            ],
            'include_dirs': [ 'include' ],
            'sources': [
                'src/smisc_anyserializer.cpp',
                'src/smisc_anyserializer.t.cpp',
                'src/smisc_classserializer.cpp',
                'src/smisc_classserializerboostserializableutil.cpp',
                'src/smisc_classserializerboostserializableutil.t.cpp',
                'src/smisc_classserializerlexicalcastutil.cpp',
                'src/smisc_classserializerutil.cpp',
                'src/smisc_classserializerutil.t.cpp',
                'src/smisc_cpptypeid.cpp',
                'src/smisc_doc.cpp',
                'src/smisc_dynamic.cpp',
                'src/smisc_lazylist.cpp',
                'src/smisc_lazytree.cpp',
                'src/smisc_normedvectorspace.cpp',
                'src/smisc_point1d.cpp',
                'src/smisc_point1dnormedvectorspace.cpp',
                'src/smisc_point1dvectorspace.cpp',
                'src/smisc_tests.cpp',
                'src/smisc_textiarchivewithanyserializer.cpp',
                'src/smisc_textoarchivewithanyserializer.cpp',
                'src/smisc_totalfiniteorder.cpp',
                'src/smisc_totalfiniteorder.t.cpp',
                'src/smisc_vectorspace.cpp',
            ],
        },
        {
            "target_name": 'stest',
            "type": 'static_library',
            'dependencies': [
                '../boost-gyp/boost.gyp:boost.headers',
                '../boost-gyp/boost.gyp:boost.serialization',
                '../boost-gyp/boost.gyp:boost.test',
            ],
            'include_dirs': [
                'include',
            ],
            'export_dependent_settings': [
                '../boost-gyp/boost.gyp:boost.test',
                '../boost-gyp/boost.gyp:boost.serialization',
            ],
            'sources': [
                'src/stest_mastersuitetestcollector.cpp',
                'src/stest_serialization.cpp',
                'src/stest_serialization.t.cpp',
                'src/stest_testcollector.cpp',
                'src/stest_testcollector.t.cpp',
                'src/stest_tests.cpp',
            ],
        },
    ],
}