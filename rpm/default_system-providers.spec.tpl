
%files %{{p_{name}}} -f {name}.files
%defattr(-,root,root,-)

%pre %{{p_{name}}}
%statefs_pre

%post %{{p_{name}}}
/sbin/ldconfig
%statefs_provider_register default {name} system
%statefs_post

%preun %{{p_{name}}}
%statefs_preun
%statefs_provider_unregister default {name} system

%postun %{{p_{name}}}
/sbin/ldconfig
%statefs_postun

%posttrans %{{p_{name}}}
%statefs_provider_register default {name} system
