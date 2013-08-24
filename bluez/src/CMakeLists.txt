find_package(Threads)

find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)

include_directories(
  ${Qt5Core_INCLUDES}
  ${Qt5DBus_INCLUDES}
)

link_directories(
  ${Qt5Core_LIBRARY_DIRS}
  ${Qt5DBus_LIBRARY_DIRS}
)

set(MGR_IF org.bluez.Manager.xml)
set(DEV_IF org.bluez.Device.xml) 
#set(ALL_IF org.bluez.all.xml)

set_source_files_properties(${MGR_IF} ${DEV_IF}
  PROPERTIES INCLUDE common.h
  )

qt5_add_dbus_interface(SRC ${MGR_IF} manager)
qt5_add_dbus_interface(SRC ${DEV_IF} device)
#qt5_add_dbus_interface(SRC ${ALL_IF} all)

add_library(provider-bluez SHARED
  ${SRC}
  provider.cpp
  )

target_link_libraries(provider-bluez
  ${CMAKE_THREAD_LIBS_INIT}
  ${STATEFS_LIBRARIES}
  ${Qt5Core_LIBRARIES}
  ${Qt5DBus_LIBRARIES}
  )

install(TARGETS provider-bluez DESTINATION ${DST_LIB}/statefs)
