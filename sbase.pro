NAME=sbase
include( ../smake/lib.pri )

## Dependencies

addBoostDependency($$BOOST_PATH)

## Sources

SOURCES += src/sbasetest_tests.cpp
SOURCES += src/stest_mastersuitetestcollector.cpp
SOURCES += src/stest_serialization.cpp
SOURCES += src/stest_testcollector.cpp
SOURCES += src/stest_tests.cpp

## Build Options

CONFIG -= qt
