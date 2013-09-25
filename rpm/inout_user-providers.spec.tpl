%files %{{p_inout_{name}}} -f inout_{name}.files
%defattr(-,root,root,-)

%posttrans %{{p_inout_{name}}}
%statefs_provider_register qt5 inout_{name} system
%statefs_provider_unregister qt5 inout_{name} user || :

%post %{{p_inout_{name}}} -p /sbin/ldconfig

%postun %{{p_inout_{name}}}
/sbin/ldconfig
%statefs_provider_unregister qt5 inout_{name} system
%statefs_cleanup || :

