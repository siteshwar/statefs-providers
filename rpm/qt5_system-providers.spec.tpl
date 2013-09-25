%files %{{p_{name}}} -f {name}.files
%defattr(-,root,root,-)

%posttrans %{{p_{name}}}
%statefs_provider_register qt5 {name} system
%statefs_provider_unregister qt5 {name} user || :

%post %{{p_{name}}} -p /sbin/ldconfig

%postun %{{p_{name}}}
/sbin/ldconfig
%statefs_provider_unregister qt5 {name} system
%statefs_cleanup || :
