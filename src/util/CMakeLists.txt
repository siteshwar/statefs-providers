set(CMAKE_AUTOMOC TRUE)

include_directories(
  ${Qt5Core_INCLUDES}
  ${Qt5DBus_INCLUDES}
)

link_directories(
  ${Qt5Core_LIBRARY_DIRS}
  ${Qt5DBus_LIBRARY_DIRS}
)


add_library(statefs-qt5
  SHARED util.cpp ns.cpp
)

target_link_libraries(statefs-qt5
  ${Qt5Core_LIBRARIES}
)

configure_file(statefs-qt5.pc.in statefs-qt5.pc @ONLY)
  
install(TARGETS statefs-qt5 DESTINATION ${DST_LIB})
install(FILES statefs-qt5.pc DESTINATION ${DST_LIB}/pkgconfig)
