NAME=sbase
include( ../smake/lib.pri )

## Dependencies

addBoostDependency($$BOOST_PATH)

# utf8 is special
INCLUDEPATH += $$UTF8_CPP_PATH/source

## Sources

SOURCES += src/sbasetest_tests.cpp
SOURCES += src/sboost_serialize.cpp
SOURCES += src/sboost_variant.cpp
SOURCES += src/smisc_cpptypeid.cpp
SOURCES += src/smisc_doc.cpp
SOURCES += src/smisc_dynamic.cpp
SOURCES += src/smisc_lazylist.cpp
SOURCES += src/smisc_lazytree.cpp
SOURCES += src/smisc_totalfiniteorder.cpp
SOURCES += src/stest_mastersuitetestcollector.cpp
SOURCES += src/stest_serialization.cpp
SOURCES += src/stest_serialization.t.cpp
SOURCES += src/stest_testcollector.cpp
SOURCES += src/stest_testcollector.t.cpp
SOURCES += src/stest_tests.cpp

## Build Options

CONFIG -= qt
