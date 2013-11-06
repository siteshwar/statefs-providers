
%files %{{p_{name}}} -f {name}.files
%defattr(-,root,root,-)

%pre %{{p_{name}}}
%statefs_pre
if [ -f %{{_statefs_libdir}}/libprovider-{old_name}.so ]; then
statefs unregister %{{_statefs_libdir}}/libprovider-{old_name}.so || :
fi

%post %{{p_{name}}}
/sbin/ldconfig
%statefs_provider_register qt5 {name} system
%statefs_post

%preun %{{p_{name}}}
%statefs_preun
%statefs_provider_unregister qt5 {name} system

%postun %{{p_{name}}}
/sbin/ldconfig
%statefs_postun
