
%files %{{p_{name}}} -f {name}.files
%defattr(-,root,root,-)

%pre %{{p_{name}}}
%statefs_pre || :

%posttrans %{{p_{name}}}
%statefs_provider_register qt5 {name}
%statefs_posttrans || :

%post %{{p_{name}}} -p /sbin/ldconfig

%preun %{{p_{name}}}
%statefs_preun || :

%postun %{{p_{name}}}
/sbin/ldconfig
%statefs_provider_unregister qt5 {name}
%statefs_cleanup
%statefs_postun || :
