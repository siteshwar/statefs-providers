
%files %{{p_{name}}} -f {name}.files
%defattr(-,root,root,-)

%pre %{{p_{name}}}
%statefs_pre

%post %{{p_{name}}}
%{{_statefs_libdir}}/provider-do unregister inout {name}
%statefs_provider_register inout {name} system
%statefs_post

%preun %{{p_{name}}}
%statefs_preun
%statefs_provider_unregister inout {name}

%postun %{{p_{name}}}
%statefs_postun
