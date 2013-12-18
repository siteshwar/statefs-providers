set(CMAKE_AUTOMOC TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

pkg_check_modules(STATEFS_UTIL statefs-util REQUIRED)

include_directories(
  ${STATEFS_UTIL_INCLUDE_DIRS}
)

link_directories(
  ${STATEFS_UTIL_LIBRARY_DIRS}
)

add_library(provider-bme SHARED
  provider_bme.cpp
  bmeipc.c
  )

target_link_libraries(provider-bme
  ${CMAKE_THREAD_LIBS_INIT}
  ${STATEFS_LIBRARIES}
  ${STATEFS_UTIL_LIBRARIES}
)

install(TARGETS provider-bme DESTINATION ${DST_LIB}/statefs)
