pkg_check_modules(STATEFS_UTIL statefs-util REQUIRED)

include_directories(
  ${STATEFS_UTIL_INCLUDE_DIRS}
)

link_directories(
  ${STATEFS_UTIL_LIBRARY_DIRS}
)

add_library(provider-back_cover SHARED
  provider_back_cover.cpp
  )

target_link_libraries(provider-back_cover
  ${STATEFS_LIBRARIES}
  ${STATEFS_UTIL_LIBRARIES}
  )

install(TARGETS provider-back_cover DESTINATION ${DST_LIB}/statefs)
