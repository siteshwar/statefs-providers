if(ENABLE_QT5)

include_directories(
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5DBus_INCLUDE_DIRS}
)

set(LIBS statefs-providers-qt5 provider-upower)

foreach(LIB ${LIBS})
  add_executable(test-linking-${LIB} ${LIB}-main.cpp ${LIB}-m2.cpp)
  target_link_libraries(test-linking-${LIB} ${LIB})
endforeach(LIB ${LIBS})

endif()
