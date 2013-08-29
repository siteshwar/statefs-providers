PROJECT(statefs-providers)
cmake_minimum_required(VERSION 2.8)

IF(NOT DEFINED STATEFS_QT_VERSION)
MESSAGE(FATAL_ERROR "Define STATEFS_QT_VERSION")
ENDIF(NOT DEFINED STATEFS_QT_VERSION)

IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
  SET(LIB_SUFFIX "")
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)
  SET(LIB_SUFFIX 64)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)

set(DST_LIB lib${LIB_SUFFIX})

find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -Wall -O2 -g -feliminate-unused-debug-types -std=c++0x"
  )

pkg_check_modules(STATEFS statefs-cpp REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${STATEFS_INCLUDES}
)

link_directories(
  ${STATEFS_LIBS}
)

install(DIRECTORY include/statefs DESTINATION include/qt5)

add_custom_target(doc
  COMMAND doxygen ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile)

find_package(Threads)

find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)

add_subdirectory(src/util)
add_subdirectory(src/bluez)
add_subdirectory(src/upower)
add_subdirectory(tests)

