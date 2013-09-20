PROJECT(statefs-inout-providers NONE)

install(FILES
  inout-bluetooth.conf
  inout-cellular.conf
  inout-keyboard.conf
  inout-mce.conf
  inout-network.conf
  inout-profile.conf
  inout-power.conf
  DESTINATION share/statefs
)
