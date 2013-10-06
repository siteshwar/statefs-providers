%{!?_with_qt5: %{!?_without_qt5: %define _with_qt5 --with-qt5}}

%define ckit_version 0.7.41
%define ckit_version1 0.7.42
%define ckit_statefs_version 0.2.30
%define ckit_statefs_version1 0.2.31
%define maemo_ver 0.7.30
%define maemo_ver1 0.7.31
%define meego_ver 0.1.0
%define meego_ver1 0.1.0.1
%define statefs_ver 0.3.17

Summary: Statefs providers
Name: statefs-providers
Version: x.x.x
Release: 1
License: LGPLv2
Group: System Environment/Libraries
URL: http://github.com/nemomobile/statefs-providers

Source0: %{name}-%{version}.tar.bz2
Source1: generate-spec.py
Source2: inout-install.spec.tpl
Source3: inout_system-providers.spec.tpl
Source4: inout_user-providers.spec.tpl
Source5: qt5-install.spec.tpl
Source6: qt5_system-providers.spec.tpl
Source7: qt5_user-providers.spec.tpl
Source8: statefs-providers.spec.tpl
BuildRequires: cmake >= 2.8
BuildRequires: statefs >= %{statefs_ver}
BuildRequires: pkgconfig(statefs-cpp) >= %{statefs_ver}
%if 0%{?_with_qt5:1}
BuildRequires:pkgconfig(Qt5Core)
BuildRequires:pkgconfig(Qt5DBus)
%endif
BuildRequires: pkgconfig(cor) >= 0.1.11

%description
%{summary}

%if 0%{?_with_qt5:1}
%define p_common -n statefs-provider-qt5
%define n_common statefs-provider-qt5
%else
%define p_common -n statefs-providers
%define n_common statefs-providers
%endif

%package %{p_common}
Summary: Package to replace contextkit plugins
Group: System Environment/Libraries
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: statefs >= %{statefs_ver}
Obsoletes: contextkit-maemo <= %{maemo_ver}
Provides: contextkit-maemo = %{maemo_ver1}
Obsoletes: contextkit-meego <= %{meego_ver}
Provides: contextkit-meego = %{meego_ver1}
Obsoletes: statefs-contextkit-provider <= %{ckit_statefs_version}
Provides: statefs-contextkit-provider = %{ckit_statefs_version1}
%{?_with_qt5:BuildRequires: pkgconfig(statefs-qt5) >= 0.2.33}
%description %{p_common}
%{summary}

%if 0%{?_with_qt5:1}
%package qt5-devel
Summary: StateFS Qt5 library for providers, development files
Group: Development/Libraries
Requires: statefs-providers-qt5 = %{version}-%{release}
%description qt5-devel
%{summary}

%define p_bluez -n statefs-provider-bluez
%define p_upower -n statefs-provider-upower
%define p_connman -n statefs-provider-connman
%define p_ofono -n statefs-provider-ofono
%define p_mce -n statefs-provider-mce
%define p_profile -n statefs-provider-profile
%define p_keyboard_generic -n statefs-provider-keyboard-generic

%endif

%define p_inout_bluetooth -n statefs-provider-inout-bluetooth
%define p_inout_power -n statefs-provider-inout-power
%define p_inout_network -n statefs-provider-inout-network
%define p_inout_cellular -n statefs-provider-inout-cellular
%define p_inout_mce -n statefs-provider-inout-mce
%define p_inout_profile -n statefs-provider-inout-profile
%define p_inout_keyboard -n statefs-provider-inout-keyboard
%define p_inout_location -n statefs-provider-inout-location

%if 0%{?_with_qt5:1}

%package %{p_bluez}
Summary: BlueZ statefs provider
Group: System Environment/Libraries
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: %{n_common} = %{version}-%{release}
Requires: statefs-loader-qt5
Obsoletes: contextkit-plugin-bluez <= %{ckit_version}
Provides: contextkit-plugin-bluez = %{ckit_version1}
Obsoletes: contextkit-plugin-bluetooth <= %{ckit_version}
Provides: contextkit-plugin-bluetooth = %{ckit_version1}
Provides: statefs-provider-bluetooth = %{version}-%{release}
%description %{p_bluez}
%{summary}

%package %{p_upower}
Summary: Upower statefs provider
Group: System Environment/Libraries
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: %{n_common} = %{version}-%{release}
Requires: statefs-loader-qt5
Requires: upower >= 0.9.18
Obsoletes: contextkit-meego-battery-upower <= %{meego_ver}
Provides: contextkit-meego-battery-upower = %{meego_ver1}
Obsoletes: contextkit-plugin-upower <= %{ckit_version}
Provides: contextkit-plugin-upower = %{ckit_version1}
Obsoletes: contextkit-plugin-power <= %{ckit_version}
Provides: contextkit-plugin-power = %{ckit_version1}
Provides: statefs-provider-power = %{version}-%{release}
%description %{p_upower}
%{summary}

%package %{p_connman}
Summary: ConnMan statefs provider
Group: System Environment/Libraries
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: %{n_common} = %{version}-%{release}
Requires: statefs-loader-qt5
Requires: connman >= 1.15
Obsoletes: contextkit-meego-internet <= %{meego_ver}
Provides: contextkit-meego-internet = %{meego_ver1}
Obsoletes: contextkit-plugin-connman <= %{ckit_version}
Provides: contextkit-plugin-connman = %{ckit_version1}
Provides: statefs-provider-internet = %{version}-%{release}
Provides: statefs-provider-network = %{version}-%{release}
%description %{p_connman}
%{summary}

%package %{p_ofono}
Summary: oFono statefs provider
Group: System Environment/Libraries
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: %{n_common} = %{version}-%{release}
Requires: statefs-loader-qt5
Requires: ofono >= 1.12
Obsoletes: contextkit-meego-cellular <= %{meego_ver}
Provides: contextkit-meego-cellular = %{meego_ver1}
Obsoletes: contextkit-meego-phone <= %{meego_ver}
Provides: contextkit-meego-phone = %{meego_ver1}
Obsoletes: contextkit-plugin-cellular <= %{ckit_version}
Provides: contextkit-plugin-cellular = %{ckit_version1}
Obsoletes: contextkit-plugin-ofono <= %{ckit_version}
Provides: contextkit-plugin-ofono = %{ckit_version1}
Provides: statefs-provider-cellular = %{version}-%{release}
%description %{p_ofono}
%{summary}

%package %{p_mce}
Summary: MCE statefs provider
Group: System Environment/Libraries
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: %{n_common} = %{version}-%{release}
Requires: statefs-loader-qt5
BuildRequires: pkgconfig(mce)
Provides: statefs-provider-system = %{version}-%{release}
Obsoletes: contextkit-maemo-mce <= %{maemo_ver}
Provides: contextkit-maemo-mce = %{maemo_ver1}
Obsoletes: contextkit-plugin-mce <= %{ckit_version}
Provides: contextkit-plugin-mce = %{ckit_version1}
%description %{p_mce}
%{summary}

%package %{p_profile}
Summary: Profiled statefs provider
Group: System Environment/Libraries
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: %{n_common} = %{version}-%{release}
Requires: statefs-loader-qt5
Requires: profiled >= 0.30
Obsoletes: contextkit-plugin-profile <= %{ckit_version}
Provides: contextkit-plugin-profile = %{ckit_version1}
%description %{p_profile}
%{summary}

%package %{p_keyboard_generic}
Summary: Generic keyboard statefs provider
Group: System Environment/Libraries
BuildRequires: pkgconfig(udev) >= 187
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: %{n_common} = %{version}-%{release}
Requires: statefs-loader-qt5
Obsoletes: contextkit-plugin-keyboard-generic <= %{ckit_version}
Provides: contextkit-plugin-keyboard-generic = %{ckit_version1}
Provides: statefs-provider-keyboard = %{version}-%{release}
%description %{p_keyboard_generic}
%{summary}

%endif
# inout providers

%package %{p_inout_bluetooth}
Summary: Statefs inout provider: bluetooth properties
Group: System Environment/Libraries
Requires: statefs >= %{statefs_ver}
Provides: statefs-provider-bluetooth = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_bluetooth}
%{summary}

%package %{p_inout_power}
Summary: Statefs inout provider: power subsystem properties
Group: System Environment/Libraries
Requires: statefs >= %{statefs_ver}
Provides: statefs-provider-power = %{version}-%{release}
Provides: statefs-provider-power-emu = 0.3.13
Obsoletes: statefs-provider-power-emu < 0.3.13
BuildArch: noarch
%description %{p_inout_power}
%{summary}

%package %{p_inout_network}
Summary: Statefs inout provider: network properties
Group: System Environment/Libraries
Requires: statefs >= %{statefs_ver}
Provides: statefs-provider-internet = %{version}-%{release}
Provides: statefs-provider-network = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_network}
%{summary}

%package %{p_inout_cellular}
Summary: Statefs inout provider: cellular network properties
Group: System Environment/Libraries
Requires: statefs >= %{statefs_ver}
Provides: statefs-provider-cellular = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_cellular}
%{summary}

%package %{p_inout_mce}
Summary: Statefs inout provider: mce properties
Group: System Environment/Libraries
Requires: statefs >= %{statefs_ver}
Provides: statefs-provider-system = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_mce}
%{summary}

%package %{p_inout_profile}
Summary: Statefs inout provider: profile information
Group: System Environment/Libraries
Requires: statefs >= %{statefs_ver}
Provides: statefs-provider-profile = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_profile}
%{summary}

%package %{p_inout_keyboard}
Summary: Statefs inout provider: keyboard information
Group: System Environment/Libraries
Requires: statefs >= %{statefs_ver}
Provides: statefs-provider-keyboard = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_keyboard}
%{summary}

%package %{p_inout_location}
Summary: Statefs inout provider: location information
Group: System Environment/Libraries
Requires: statefs >= %{statefs_ver}
Provides: statefs-provider-location = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_location}
%{summary}


%prep
%setup -q


%build
%cmake -DSTATEFS_QT_VERSION=%{version} %{?_with_multiarch:-DENABLE_MULTIARCH=ON} %{?_without_qt5:-DENABLE_QT5=OFF}
make %{?jobs:-j%jobs}
make doc
pushd inout && %cmake && popd


%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}
pushd inout && make install DESTDIR=%{buildroot} && popd

%if 0%{?_with_qt5:1}
%statefs_provider_install qt5 bluez %{_statefs_libdir}/libprovider-bluez.so system%statefs_provider_install qt5 upower %{_statefs_libdir}/libprovider-upower.so system%statefs_provider_install qt5 connman %{_statefs_libdir}/libprovider-connman.so system%statefs_provider_install qt5 ofono %{_statefs_libdir}/libprovider-ofono.so system%statefs_provider_install qt5 mce %{_statefs_libdir}/libprovider-mce.so system%statefs_provider_install qt5 keyboard_generic %{_statefs_libdir}/libprovider-keyboard_generic.so system
%statefs_provider_install qt5 profile %{_statefs_libdir}/libprovider-profile.so user
%endif

%statefs_provider_install inout inout_bluetooth %{_statefs_datadir}/inout_bluetooth.conf system
%statefs_provider_install inout inout_power %{_statefs_datadir}/inout_power.conf system
%statefs_provider_install inout inout_network %{_statefs_datadir}/inout_network.conf system
%statefs_provider_install inout inout_cellular %{_statefs_datadir}/inout_cellular.conf system
%statefs_provider_install inout inout_mce %{_statefs_datadir}/inout_mce.conf system
%statefs_provider_install inout inout_keyboard %{_statefs_datadir}/inout_keyboard.conf system
%statefs_provider_install inout inout_location %{_statefs_datadir}/inout_location.conf system

%statefs_provider_install inout inout_profile %{_statefs_datadir}/inout_profile.conf user


%clean
rm -rf %{buildroot}

%files %{p_common}
%defattr(-,root,root,-)
%doc README
%if 0%{?_with_qt5:1}
%{_libdir}/libstatefs-providers-qt5.so
%endif

%post %{p_common} -p /sbin/ldconfig
%postun %{p_common} -p /sbin/ldconfig

%if 0%{?_with_qt5:1}

%files qt5-devel
%defattr(-,root,root,-)
%{_qt5_headerdir}/statefs/qt/*.hpp
%{_libdir}/pkgconfig/statefs-providers-qt5.pc


%files %{p_bluez} -f bluez.files
%defattr(-,root,root,-)

%pre %{p_bluez}
%statefs_pre || :

%posttrans %{p_bluez}
%statefs_provider_register qt5 bluez system
statefs unregister %{_statefs_libdir}/libprovider-bluez.so
%statefs_posttrans || :

%post %{p_bluez} -p /sbin/ldconfig

%preun %{p_bluez}
%statefs_preun || :

%postun %{p_bluez}
/sbin/ldconfig
%statefs_provider_unregister qt5 bluez system
%statefs_cleanup
%statefs_postun || :

%files %{p_upower} -f upower.files
%defattr(-,root,root,-)

%pre %{p_upower}
%statefs_pre || :

%posttrans %{p_upower}
%statefs_provider_register qt5 upower system
statefs unregister %{_statefs_libdir}/libprovider-upower.so
%statefs_posttrans || :

%post %{p_upower} -p /sbin/ldconfig

%preun %{p_upower}
%statefs_preun || :

%postun %{p_upower}
/sbin/ldconfig
%statefs_provider_unregister qt5 upower system
%statefs_cleanup
%statefs_postun || :

%files %{p_connman} -f connman.files
%defattr(-,root,root,-)

%pre %{p_connman}
%statefs_pre || :

%posttrans %{p_connman}
%statefs_provider_register qt5 connman system
statefs unregister %{_statefs_libdir}/libprovider-connman.so
%statefs_posttrans || :

%post %{p_connman} -p /sbin/ldconfig

%preun %{p_connman}
%statefs_preun || :

%postun %{p_connman}
/sbin/ldconfig
%statefs_provider_unregister qt5 connman system
%statefs_cleanup
%statefs_postun || :

%files %{p_ofono} -f ofono.files
%defattr(-,root,root,-)

%pre %{p_ofono}
%statefs_pre || :

%posttrans %{p_ofono}
%statefs_provider_register qt5 ofono system
statefs unregister %{_statefs_libdir}/libprovider-ofono.so
%statefs_posttrans || :

%post %{p_ofono} -p /sbin/ldconfig

%preun %{p_ofono}
%statefs_preun || :

%postun %{p_ofono}
/sbin/ldconfig
%statefs_provider_unregister qt5 ofono system
%statefs_cleanup
%statefs_postun || :

%files %{p_mce} -f mce.files
%defattr(-,root,root,-)

%pre %{p_mce}
%statefs_pre || :

%posttrans %{p_mce}
%statefs_provider_register qt5 mce system
statefs unregister %{_statefs_libdir}/libprovider-mce.so
%statefs_posttrans || :

%post %{p_mce} -p /sbin/ldconfig

%preun %{p_mce}
%statefs_preun || :

%postun %{p_mce}
/sbin/ldconfig
%statefs_provider_unregister qt5 mce system
%statefs_cleanup
%statefs_postun || :

%files %{p_keyboard_generic} -f keyboard_generic.files
%defattr(-,root,root,-)

%pre %{p_keyboard_generic}
%statefs_pre || :

%posttrans %{p_keyboard_generic}
%statefs_provider_register qt5 keyboard_generic system
statefs unregister %{_statefs_libdir}/libprovider-keyboard-generic.so
%statefs_posttrans || :

%post %{p_keyboard_generic} -p /sbin/ldconfig

%preun %{p_keyboard_generic}
%statefs_preun || :

%postun %{p_keyboard_generic}
/sbin/ldconfig
%statefs_provider_unregister qt5 keyboard_generic system
%statefs_cleanup
%statefs_postun || :


%files %{p_profile} -f profile.files
%defattr(-,root,root,-)

%pre %{p_profile}
%statefs_pre || :

%posttrans %{p_profile}
%statefs_provider_register qt5 profile
%statefs_posttrans || :

%post %{p_profile} -p /sbin/ldconfig

%preun %{p_profile}
%statefs_preun || :

%postun %{p_profile}
/sbin/ldconfig
%statefs_provider_unregister qt5 profile
%statefs_cleanup
%statefs_postun || :


%endif


%files %{p_inout_bluetooth} -f inout_bluetooth.files
%defattr(-,root,root,-)

%pre %{p_inout_bluetooth}
%statefs_pre || :

%posttrans %{p_inout_bluetooth}
%statefs_provider_register inout inout_bluetooth system
%statefs_provider_unregister inout inout_bluetooth user
%statefs_posttrans || :

%post %{p_inout_bluetooth} -p /sbin/ldconfig

%preun %{p_inout_bluetooth}
%statefs_preun || :

%postun %{p_inout_bluetooth}
/sbin/ldconfig
%statefs_provider_unregister inout inout_bluetooth system
%statefs_cleanup
%statefs_postun || :

%files %{p_inout_power} -f inout_power.files
%defattr(-,root,root,-)

%pre %{p_inout_power}
%statefs_pre || :

%posttrans %{p_inout_power}
%statefs_provider_register inout inout_power system
%statefs_provider_unregister inout inout_power user
%statefs_posttrans || :

%post %{p_inout_power} -p /sbin/ldconfig

%preun %{p_inout_power}
%statefs_preun || :

%postun %{p_inout_power}
/sbin/ldconfig
%statefs_provider_unregister inout inout_power system
%statefs_cleanup
%statefs_postun || :

%files %{p_inout_network} -f inout_network.files
%defattr(-,root,root,-)

%pre %{p_inout_network}
%statefs_pre || :

%posttrans %{p_inout_network}
%statefs_provider_register inout inout_network system
%statefs_provider_unregister inout inout_network user
%statefs_posttrans || :

%post %{p_inout_network} -p /sbin/ldconfig

%preun %{p_inout_network}
%statefs_preun || :

%postun %{p_inout_network}
/sbin/ldconfig
%statefs_provider_unregister inout inout_network system
%statefs_cleanup
%statefs_postun || :

%files %{p_inout_cellular} -f inout_cellular.files
%defattr(-,root,root,-)

%pre %{p_inout_cellular}
%statefs_pre || :

%posttrans %{p_inout_cellular}
%statefs_provider_register inout inout_cellular system
%statefs_provider_unregister inout inout_cellular user
%statefs_posttrans || :

%post %{p_inout_cellular} -p /sbin/ldconfig

%preun %{p_inout_cellular}
%statefs_preun || :

%postun %{p_inout_cellular}
/sbin/ldconfig
%statefs_provider_unregister inout inout_cellular system
%statefs_cleanup
%statefs_postun || :

%files %{p_inout_mce} -f inout_mce.files
%defattr(-,root,root,-)

%pre %{p_inout_mce}
%statefs_pre || :

%posttrans %{p_inout_mce}
%statefs_provider_register inout inout_mce system
%statefs_provider_unregister inout inout_mce user
%statefs_posttrans || :

%post %{p_inout_mce} -p /sbin/ldconfig

%preun %{p_inout_mce}
%statefs_preun || :

%postun %{p_inout_mce}
/sbin/ldconfig
%statefs_provider_unregister inout inout_mce system
%statefs_cleanup
%statefs_postun || :

%files %{p_inout_keyboard} -f inout_keyboard.files
%defattr(-,root,root,-)

%pre %{p_inout_keyboard}
%statefs_pre || :

%posttrans %{p_inout_keyboard}
%statefs_provider_register inout inout_keyboard system
%statefs_provider_unregister inout inout_keyboard user
%statefs_posttrans || :

%post %{p_inout_keyboard} -p /sbin/ldconfig

%preun %{p_inout_keyboard}
%statefs_preun || :

%postun %{p_inout_keyboard}
/sbin/ldconfig
%statefs_provider_unregister inout inout_keyboard system
%statefs_cleanup
%statefs_postun || :

%files %{p_inout_location} -f inout_location.files
%defattr(-,root,root,-)

%pre %{p_inout_location}
%statefs_pre || :

%posttrans %{p_inout_location}
%statefs_provider_register inout inout_location system
%statefs_provider_unregister inout inout_location user
%statefs_posttrans || :

%post %{p_inout_location} -p /sbin/ldconfig

%preun %{p_inout_location}
%statefs_preun || :

%postun %{p_inout_location}
/sbin/ldconfig
%statefs_provider_unregister inout inout_location system
%statefs_cleanup
%statefs_postun || :


%files %{p_inout_profile} -f inout_profile.files
%defattr(-,root,root,-)

%pre %{p_inout_profile}
%statefs_pre || :

%posttrans %{p_inout_profile}
%statefs_provider_register inout inout_profile system
%statefs_provider_unregister inout inout_profile user
%statefs_posttrans || :

%post %{p_inout_profile} -p /sbin/ldconfig

%preun %{p_inout_profile}
%statefs_preun || :

%postun %{p_inout_profile}
/sbin/ldconfig
%statefs_provider_unregister inout inout_profile system
%statefs_cleanup
%statefs_postun || :

