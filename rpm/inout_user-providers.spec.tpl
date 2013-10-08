
%files %{{p_{name}}} -f {name}.files
%defattr(-,root,root,-)

%pre %{{p_{name}}}
%statefs_pre

%post %{{p_{name}}}
%statefs_provider_register inout {name} system
%statefs_post

%preun %{{p_{name}}}
%statefs_preun
%statefs_provider_unregister inout {name} user

%postun %{{p_{name}}}
%statefs_postun
