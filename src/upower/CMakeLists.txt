find_package(Threads)

find_package(Qt5Core REQUIRED)
find_package(Qt5DBus REQUIRED)

set(CMAKE_AUTOMOC TRUE)

include_directories(
  ${Qt5Core_INCLUDES}
  ${Qt5DBus_INCLUDES}
)

link_directories(
  ${Qt5Core_LIBRARY_DIRS}
  ${Qt5DBus_LIBRARY_DIRS}
)

set(DEV_IF DeviceKit.Power.Device.xml)
set(MGR_IF DeviceKit.Power.xml) 

# set_source_files_properties(${MGR_IF} ${DEV_IF}
#   PROPERTIES INCLUDE common.h
#   )
set_source_files_properties(${MGR_IF} ${DEV_IF}
  PROPERTIES NO_NAMESPACE TRUE
  )

 

qt5_add_dbus_interface(SRC ${MGR_IF} manager_interface)
qt5_add_dbus_interface(SRC ${DEV_IF} device_interface)

add_library(provider-upower SHARED
  ${SRC}
  upower.cpp
  )

target_link_libraries(provider-upower
  statefs-qt5
  ${CMAKE_THREAD_LIBS_INIT}
  ${STATEFS_LIBRARIES}
  ${Qt5Core_LIBRARIES}
  ${Qt5DBus_LIBRARIES}
  )

install(TARGETS provider-upower DESTINATION ${DST_LIB}/statefs)
