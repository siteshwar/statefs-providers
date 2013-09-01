set(CMAKE_AUTOMOC TRUE)

include_directories(
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5DBus_INCLUDE_DIRS}
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
#qt5_wrap_cpp(STATEFS_QT_SRC ${STATEFS_QT_HEADERS})

add_library(provider-upower SHARED
  ${SRC}
#  ${STATEFS_QT_SRC}
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
