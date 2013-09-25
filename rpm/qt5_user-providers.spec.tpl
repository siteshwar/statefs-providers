%files %{{p_{name}}} -f {name}.files
%defattr(-,root,root,-)

%posttrans %{{p_{name}}}
%statefs_provider_register qt5 {name} || :

%post %{{p_{name}}} -p /sbin/ldconfig

%postun %{{p_{name}}}
/sbin/ldconfig
%statefs_provider_unregister qt5 {name}
%statefs_cleanup || :

