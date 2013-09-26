PROJECT(statefs-inout-providers NONE)

install(FILES
  inout_bluetooth.conf
  inout_cellular.conf
  inout_keyboard.conf
  inout_mce.conf
  inout_network.conf
  inout_profile.conf
  inout_power.conf
  inout_location.conf
  DESTINATION share/statefs
)
