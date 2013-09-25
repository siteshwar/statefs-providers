set(CMAKE_AUTOMOC TRUE)
set(SELF keyboard_generic)

pkg_check_modules(UDEV libudev REQUIRED)
pkg_check_modules(COR cor REQUIRED)

include_directories(
  ${UDEV_INCLUDE_DIRS}
  ${COR_INCLUDE_DIRS}
)

link_directories(
  ${UDEV_LIBRARY_DIRS}
  ${COR_LIBRARY_DIRS}
)

add_library(provider-${SELF} SHARED
  provider_${SELF}.cpp
  )

target_link_libraries(provider-${SELF}
  statefs-providers-qt5
  ${CMAKE_THREAD_LIBS_INIT}
  ${STATEFS_LIBRARIES}
  ${UDEV_LIBRARIES}
  ${COR_LIBRARIES}
  )

install(TARGETS provider-${SELF} DESTINATION ${DST_LIB}/statefs)
