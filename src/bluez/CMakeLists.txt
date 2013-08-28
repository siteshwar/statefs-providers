set(CMAKE_AUTOMOC TRUE)

include_directories(
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5DBus_INCLUDE_DIRS}
)

link_directories(
  ${Qt5Core_LIBRARY_DIRS}
  ${Qt5DBus_LIBRARY_DIRS}
)

set(MGR_IF org.bluez.Manager.xml)
set(ADA_IF org.bluez.Adapter.xml) 
#set(ALL_IF org.bluez.all.xml)

set_source_files_properties(${MGR_IF} ${ADA_IF}
  PROPERTIES INCLUDE common.h
  )

qt5_add_dbus_interface(SRC ${MGR_IF} manager_interface)
qt5_add_dbus_interface(SRC ${ADA_IF} adapter_interface)
#qt5_add_dbus_interface(SRC ${ALL_IF} all)

add_library(provider-bluez SHARED
  ${SRC}
  bluez.cpp
  )

target_link_libraries(provider-bluez
  statefs-qt5
  ${CMAKE_THREAD_LIBS_INIT}
  ${STATEFS_LIBRARIES}
  ${Qt5Core_LIBRARIES}
  ${Qt5DBus_LIBRARIES}
  )

install(TARGETS provider-bluez DESTINATION ${DST_LIB}/statefs)
