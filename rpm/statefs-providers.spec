%define ckit_version 0.7.41
%define ckit_version1 0.7.42
%define ckit_statefs_version 0.2.29
%define ckit_statefs_version1 0.2.30
%define maemo_ver 0.7.30
%define maemo_ver1 0.7.31
%define meego_ver 0.1.0
%define meego_ver1 0.1.0.1
%define statefs_ver 0.3.8

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

%description
%{summary}

%define p_common -n statefs-providers-common

%package %{p_common}
Summary: Package to replace contextkit plugins
Group: Applications/System
Requires: statefs >= %{statefs_ver}
Obsoletes: contextkit-maemo <= %{maemo_ver}
Provides: contextkit-maemo = %{maemo_ver1}
Obsoletes: contextkit-meego <= %{meego_ver}
Provides: contextkit-meego = %{meego_ver1}
Obsoletes: statefs-contextkit-provider <= %{ckit_statefs_version}
Provides: statefs-contextkit-provider = %{ckit_statefs_version1}
%description %{p_common}
%{summary}

%package qt5
Summary: StateFS Qt5 library for providers
Group: System Environment/Libraries
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: statefs >= %{statefs_ver}
BuildRequires: pkgconfig(statefs-qt5) >= 0.2.33
%description qt5
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

%package %{p_bluez}
Summary: BlueZ statefs provider
Group: Applications/System
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: statefs-providers-common = %{version}
Requires: statefs-loader-qt5
Obsoletes: contextkit-plugin-bluez <= %{ckit_version}
Provides: contextkit-plugin-bluez = %{ckit_version1}
Obsoletes: contextkit-plugin-bluetooth <= %{ckit_version}
Provides: contextkit-plugin-bluetooth = %{ckit_version1}
Provides: statefs-provider-bluetooth = %{version}
%description %{p_bluez}
%{summary}

%package %{p_upower}
Summary: Upower statefs provider
Group: Applications/System
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: statefs-providers-common = %{version}
Requires: statefs-loader-qt5
Requires: upower >= 0.9.18
Obsoletes: contextkit-meego-battery-upower <= %{meego_ver}
Provides: contextkit-meego-battery-upower = %{meego_ver1}
Obsoletes: contextkit-plugin-upower <= %{ckit_version}
Provides: contextkit-plugin-upower = %{ckit_version1}
Obsoletes: contextkit-plugin-power <= %{ckit_version}
Provides: contextkit-plugin-power = %{ckit_version1}
Provides: statefs-provider-power = %{version}
%description %{p_upower}
%{summary}

%package %{p_connman}
Summary: ConnMan statefs provider
Group: Applications/System
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: statefs-providers-common = %{version}
Requires: statefs-loader-qt5
Requires: connman >= 1.15
Obsoletes: contextkit-meego-internet <= %{meego_ver}
Provides: contextkit-meego-internet = %{meego_ver1}
Obsoletes: contextkit-plugin-connman <= %{ckit_version}
Provides: contextkit-plugin-connman = %{ckit_version1}
Provides: statefs-provider-internet = %{version}
%description %{p_connman}
%{summary}

%package %{p_ofono}
Summary: oFono statefs provider
Group: Applications/System
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: statefs-providers-common = %{version}
Requires: statefs-loader-qt5
Requires: connman >= 1.15
Obsoletes: contextkit-meego-cellular <= %{meego_ver}
Provides: contextkit-meego-cellular = %{meego_ver1}
Obsoletes: contextkit-plugin-cellular <= %{ckit_version}
Provides: contextkit-plugin-cellular = %{ckit_version1}
Provides: statefs-provider-cellular = %{version}
%description %{p_ofono}
%{summary}

%prep
%setup -q

%build
%cmake -DSTATEFS_QT_VERSION=%{version}
make %{?jobs:-j%jobs}
make doc

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

%clean
rm -rf %{buildroot}

%files %{p_common}
%defattr(-,root,root,-)
%doc README

%files qt5
%defattr(-,root,root,-)
%{_libdir}/libstatefs-providers-qt5.so

%post qt5 -p /sbin/ldconfig
%postun qt5 -p /sbin/ldconfig

%files qt5-devel
%defattr(-,root,root,-)
%{_qt5_headerdir}/statefs/qt/*.hpp
%{_libdir}/pkgconfig/statefs-providers-qt5.pc

%files %{p_bluez}
%defattr(-,root,root,-)
%{_libdir}/statefs/libprovider-bluez.so

%post %{p_bluez}
/sbin/ldconfig
statefs register %{_libdir}/statefs/libprovider-bluez.so --statefs-type=qt5 || :

%postun %{p_bluez}
/sbin/ldconfig

%files %{p_upower}
%defattr(-,root,root,-)
%{_libdir}/statefs/libprovider-upower.so

%post %{p_upower}
/sbin/ldconfig
statefs register %{_libdir}/statefs/libprovider-upower.so --statefs-type=qt5 || :

%postun %{p_upower}
/sbin/ldconfig

%files %{p_connman}
%defattr(-,root,root,-)
%{_libdir}/statefs/libprovider-connman.so

%post %{p_connman}
/sbin/ldconfig
statefs register %{_libdir}/statefs/libprovider-connman.so --statefs-type=qt5 || :

%postun %{p_connman}
/sbin/ldconfig

%files %{p_ofono}
%defattr(-,root,root,-)
%{_libdir}/statefs/libprovider-ofono.so

%post %{p_ofono}
/sbin/ldconfig
statefs register %{_libdir}/statefs/libprovider-ofono.so --statefs-type=qt5 || :

%postun %{p_ofono}
/sbin/ldconfig
