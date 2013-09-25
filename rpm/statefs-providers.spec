%define ckit_version 0.7.41
%define ckit_version1 0.7.42
%define ckit_statefs_version 0.2.30
%define ckit_statefs_version1 0.2.31
%define maemo_ver 0.7.30
%define maemo_ver1 0.7.31
%define meego_ver 0.1.0
%define meego_ver1 0.1.0.1
%define statefs_ver 0.3.13

Summary: Statefs providers
Name: statefs-providers
Version: x.x.x
Release: 1
License: LGPLv2
Group: System Environment/Libraries
URL: http://github.com/nemomobile/statefs-providers
Source0: %{name}-%{version}.tar.bz2
BuildRequires: cmake >= 2.8
BuildRequires: pkgconfig(statefs-cpp) >= %{statefs_ver}
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(cor) >= 0.1.8

%description
%{summary}

%define p_common -n statefs-provider-qt5
%define n_common statefs-provider-qt5

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
BuildRequires: pkgconfig(statefs-qt5) >= 0.2.33
%description %{p_common}
%{summary}

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
%define p_keyboard -n statefs-provider-keyboard-generic
# location provider placeholder
#%define p_keyboard -n statefs-provider-location-geoclue

%define p_inout_bluetooth -n statefs-provider-inout-bluetooth
%define p_inout_power -n statefs-provider-inout-power
%define p_inout_network -n statefs-provider-inout-network
%define p_inout_cellular -n statefs-provider-inout-cellular
%define p_inout_mce -n statefs-provider-inout-mce
%define p_inout_profile -n statefs-provider-inout-profile
%define p_inout_keyboard -n statefs-provider-inout-keyboard
%define p_inout_location -n statefs-provider-inout-location

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

%package %{p_keyboard}
Summary: Generic keyboard statefs provider
Group: System Environment/Libraries
BuildRequires: pkgconfig(udev) >= 187
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: %{n_common} = %{version}-%{release}
Requires: statefs-loader-qt5
Obsoletes: contextkit-plugin-keyboard-generic <= %{ckit_version}
Provides: contextkit-plugin-keyboard-generic = %{ckit_version1}
Provides: statefs-provider-keyboard-%{version}-%{release}
%description %{p_keyboard}
%{summary}

# location provider placeholder

#%package %{p_location}
#Summary: Statefs provider: geoclue location information
#Group: System Environment/Libraries
#Requires: statefs = %{statefs_ver}
#Provides: statefs-provider-location = %{version}-%{release}
#Obsoletes: contextkit-meego-location-geoclue <= %{meego_ver}
#Provides: contextkit-meego-location-geoclue = %{meego_ver1}
#Obsoletes: contextkit-plugin-location-gypsy <= %{ckit_version}
#Provides: contextkit-plugin-location-gypsy = %{ckit_version1}
#Obsoletes: contextkit-plugin-location-skyhook <= %{ckit_version}
#Provides: contextkit-plugin-location-skyhook = %{ckit_version1}
#BuildArch: noarch
#%description %{p_location}
#%{summary}
#

# inout providers

%package %{p_inout_bluetooth}
Summary: Statefs inout provider: bluetooth properties
Group: System Environment/Libraries
Requires: statefs = %{statefs_ver}
Provides: statefs-provider-bluetooth = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_bluetooth}
%{summary}

%package %{p_inout_power}
Summary: Statefs inout provider: power subsystem properties
Group: System Environment/Libraries
Requires: statefs = %{statefs_ver}
Provides: statefs-provider-power = %{version}-%{release}
Provides: statefs-provider-power-emu = 0.3.13
Obsoletes: statefs-provider-power-emu < 0.3.13
BuildArch: noarch
%description %{p_inout_power}
%{summary}

%package %{p_inout_network}
Summary: Statefs inout provider: network properties
Group: System Environment/Libraries
Requires: statefs = %{statefs_ver}
Provides: statefs-provider-internet = %{version}-%{release}
Provides: statefs-provider-network = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_network}
%{summary}

%package %{p_inout_cellular}
Summary: Statefs inout provider: cellular network properties
Group: System Environment/Libraries
Requires: statefs = %{statefs_ver}
Provides: statefs-provider-cellular = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_cellular}
%{summary}

%package %{p_inout_mce}
Summary: Statefs inout provider: mce properties
Group: System Environment/Libraries
Requires: statefs = %{statefs_ver}
Provides: statefs-provider-mce = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_mce}
%{summary}

%package %{p_inout_profile}
Summary: Statefs inout provider: profile information
Group: System Environment/Libraries
Requires: statefs = %{statefs_ver}
Provides: statefs-provider-profile = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_profile}
%{summary}

%package %{p_inout_keyboard}
Summary: Statefs inout provider: keyboard information
Group: System Environment/Libraries
Requires: statefs = %{statefs_ver}
Provides: statefs-provider-keyboard = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_keyboard}
%{summary}

%package %{p_inout_location}
Summary: Statefs inout provider: location information
Group: System Environment/Libraries
Requires: statefs = %{statefs_ver}
Provides: statefs-provider-location = %{version}-%{release}
BuildArch: noarch
%description %{p_inout_location}
%{summary}


%prep
%setup -q


%build
%cmake -DSTATEFS_QT_VERSION=%{version}
make %{?jobs:-j%jobs}
make doc
pushd inout && %cmake && popd


%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}
pushd inout && make install DESTDIR=%{buildroot} && popd


%clean
rm -rf %{buildroot}


%files %{p_common}
%defattr(-,root,root,-)
%doc README
%{_libdir}/libstatefs-providers-qt5.so

%post %{p_common} -p /sbin/ldconfig
%postun %{p_common} -p /sbin/ldconfig

%files qt5-devel
%defattr(-,root,root,-)
%{_qt5_headerdir}/statefs/qt/*.hpp
%{_libdir}/pkgconfig/statefs-providers-qt5.pc

%files %{p_bluez}
%defattr(-,root,root,-)
%{_statefs_libdir}/libprovider-bluez.so

%posttrans %{p_bluez}
%statefs_register qt5 %{_statefs_libdir}/libprovider-bluez.so || :

%post %{p_bluez}
/sbin/ldconfig

%postun %{p_bluez}
/sbin/ldconfig
%statefs_cleanup || :

%files %{p_upower}
%defattr(-,root,root,-)
%{_statefs_libdir}/libprovider-upower.so

%posttrans %{p_upower}
%statefs_register qt5 %{_statefs_libdir}/libprovider-upower.so || :

%post %{p_upower}
/sbin/ldconfig

%postun %{p_upower}
/sbin/ldconfig
%statefs_cleanup || :

%files %{p_connman}
%defattr(-,root,root,-)
%{_statefs_libdir}/libprovider-connman.so

%posttrans %{p_connman}
%statefs_register qt5 %{_statefs_libdir}/libprovider-connman.so || :

%post %{p_connman}
/sbin/ldconfig

%postun %{p_connman}
/sbin/ldconfig
%statefs_cleanup || :

%files %{p_ofono}
%defattr(-,root,root,-)
%{_statefs_libdir}/libprovider-ofono.so

%posttrans %{p_ofono}
%statefs_register qt5 %{_statefs_libdir}/libprovider-ofono.so || :

%post %{p_ofono}
/sbin/ldconfig

%postun %{p_ofono}
/sbin/ldconfig
%statefs_cleanup || :

%files %{p_mce}
%defattr(-,root,root,-)
%{_statefs_libdir}/libprovider-mce.so

%posttrans %{p_mce}
%statefs_register qt5 %{_statefs_libdir}/libprovider-mce.so || :

%post %{p_mce}
/sbin/ldconfig

%postun %{p_mce}
/sbin/ldconfig
%statefs_cleanup || :

%files %{p_profile}
%defattr(-,root,root,-)
%{_statefs_libdir}/libprovider-profile.so

%posttrans %{p_profile}
%statefs_register qt5 %{_statefs_libdir}/libprovider-profile.so || :

%post %{p_profile}
/sbin/ldconfig

%postun %{p_profile}
/sbin/ldconfig
%statefs_cleanup || :

%files %{p_keyboard}
%defattr(-,root,root,-)
%{_statefs_libdir}/libprovider-keyboard-generic.so

%posttrans %{p_keyboard}
%statefs_register qt5 %{_statefs_libdir}/libprovider-keyboard-generic.so || :

%post %{p_keyboard}
/sbin/ldconfig

%postun %{p_keyboard}
/sbin/ldconfig
%statefs_cleanup || :

# inout providers

%files %{p_inout_bluetooth}
%defattr(-,root,root,-)
%{_statefs_datadir}/inout-bluetooth.conf

%posttrans %{p_inout_bluetooth}
%statefs_register inout %{_statefs_datadir}/inout-bluetooth.conf || :

%postun %{p_inout_bluetooth}
%statefs_cleanup || :

%files %{p_inout_power}
%defattr(-,root,root,-)
%{_statefs_datadir}/inout-power.conf

%posttrans %{p_inout_power}
%statefs_register inout %{_statefs_datadir}/inout-power.conf || :

%postun %{p_inout_power}
%statefs_cleanup || :

%files %{p_inout_network}
%defattr(-,root,root,-)
%{_statefs_datadir}/inout-network.conf

%posttrans %{p_inout_network}
%statefs_register inout %{_statefs_datadir}/inout-network.conf || :

%postun %{p_inout_network}
%statefs_cleanup || :

%files %{p_inout_cellular}
%defattr(-,root,root,-)
%{_statefs_datadir}/inout-cellular.conf

%posttrans %{p_inout_cellular}
%statefs_register inout %{_statefs_datadir}/inout-cellular.conf || :

%postun %{p_inout_cellular}
%statefs_cleanup || :

%files %{p_inout_mce}
%defattr(-,root,root,-)
%{_statefs_datadir}/inout-mce.conf

%posttrans %{p_inout_mce}
%statefs_register inout %{_statefs_datadir}/inout-mce.conf || :

%postun %{p_inout_mce}
%statefs_cleanup || :

%files %{p_inout_profile}
%defattr(-,root,root,-)
%{_statefs_datadir}/inout-profile.conf

%posttrans %{p_inout_profile}
%statefs_register inout %{_statefs_datadir}/inout-profile.conf || :

%postun %{p_inout_profile}
%statefs_cleanup || :

%files %{p_inout_keyboard}
%defattr(-,root,root,-)
%{_statefs_datadir}/inout-keyboard.conf

%posttrans %{p_inout_keyboard}
%statefs_register inout %{_statefs_datadir}/inout-keyboard.conf || :

%postun %{p_inout_keyboard}
%statefs_cleanup || :

%files %{p_inout_location}
%defattr(-,root,root,-)
%{_statefs_datadir}/inout-location.conf

%posttrans %{p_inout_location}
%statefs_register inout %{_statefs_datadir}/inout-location.conf || :

%postun %{p_inout_location}
%statefs_cleanup || :

