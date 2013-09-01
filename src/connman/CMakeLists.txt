set(CMAKE_AUTOMOC TRUE)
set(SELF connman)

include_directories(
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5DBus_INCLUDE_DIRS}
)

link_directories(
  ${Qt5Core_LIBRARY_DIRS}
  ${Qt5DBus_LIBRARY_DIRS}
)

set(MGR_IF net.connman.manager.xml) 
set(ALL_IF ${MGR_IF})
set_source_files_properties(${ALL_IF}
  PROPERTIES INCLUDE dbus_types.hpp
  )
set_source_files_properties(${ALL_IF}
  PROPERTIES NO_NAMESPACE TRUE
  )

 
qt5_add_dbus_interface(SRC ${MGR_IF} manager_interface)
# qt5_wrap_cpp(STATEFS_QT_SRC ${STATEFS_QT_HEADERS})

add_library(provider-${SELF} SHARED
  ${SRC}
  # ${STATEFS_QT_SRC}
  provider_${SELF}.cpp
  )

target_link_libraries(provider-${SELF}
  statefs-qt5
  ${CMAKE_THREAD_LIBS_INIT}
  ${STATEFS_LIBRARIES}
  ${Qt5Core_LIBRARIES}
  ${Qt5DBus_LIBRARIES}
  )

install(TARGETS provider-${SELF} DESTINATION ${DST_LIB}/statefs)
