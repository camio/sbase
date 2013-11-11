NAME=sbase
include( ../smake/lib.pri )

## Dependencies

addBoostDependency($$BOOST_PATH)

# utf8 is special
INCLUDEPATH += $$UTF8_CPP_PATH/source

## Sources

SOURCES += src/sbasetest_tests.cpp
SOURCES += src/sboost_fusionatc.cpp
SOURCES += src/sboost_fusionfoldr.cpp
SOURCES += src/sboost_optional.cpp
SOURCES += src/sboost_serialize.cpp
SOURCES += src/sboost_variant.cpp
SOURCES += src/scpp_misc.cpp
SOURCES += src/scpp_string.cpp
SOURCES += src/scpp_set.cpp
SOURCES += src/scpp_utility.cpp
SOURCES += src/sfp_cf.cpp
SOURCES += src/sfp_cf.t.cpp
SOURCES += src/sfp_ciof.cpp
SOURCES += src/sfp_ciof.t.cpp
SOURCES += src/sfp_flip.cpp
SOURCES += src/sfp_tests.cpp
SOURCES += src/sfrp_behavior.cpp
SOURCES += src/sfrp_normedvectorspaceutil.cpp
SOURCES += src/sfrp_util.cpp
SOURCES += src/sfrp_util.t.cpp
SOURCES += src/sfrp_vectorspaceutil.cpp
SOURCES += src/smisc_anyserializer.cpp
SOURCES += src/smisc_anyserializer.t.cpp
SOURCES += src/smisc_cpptypeid.cpp
SOURCES += src/smisc_classserializer.cpp
SOURCES += src/smisc_classserializerlexicalcastutil.cpp
SOURCES += src/smisc_classserializerboostserializableutil.cpp
SOURCES += src/smisc_classserializerboostserializableutil.t.cpp
SOURCES += src/smisc_classserializerutil.cpp
SOURCES += src/smisc_classserializerutil.t.cpp
SOURCES += src/smisc_doc.cpp
SOURCES += src/smisc_dynamic.cpp
SOURCES += src/smisc_lazylist.cpp
SOURCES += src/smisc_lazytree.cpp
SOURCES += src/smisc_normedvectorspace.cpp
SOURCES += src/smisc_point1d.cpp
SOURCES += src/smisc_point1dnormedvectorspace.cpp
SOURCES += src/smisc_point1dvectorspace.cpp
SOURCES += src/smisc_tests.cpp
SOURCES += src/smisc_textiarchivewithanyserializer.cpp
SOURCES += src/smisc_textoarchivewithanyserializer.cpp
SOURCES += src/smisc_totalfiniteorder.cpp
SOURCES += src/smisc_vectorspace.cpp
SOURCES += src/stest_mastersuitetestcollector.cpp
SOURCES += src/stest_serialization.cpp
SOURCES += src/stest_serialization.t.cpp
SOURCES += src/stest_testcollector.cpp
SOURCES += src/stest_testcollector.t.cpp
SOURCES += src/stest_tests.cpp

## Build Options

CONFIG -= qt
