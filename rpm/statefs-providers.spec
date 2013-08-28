%define ckit_version 0.7.41
%define ckit_statefs_version 0.2.29
%define maemo_ver 0.7.30
%define maemo_ver1 0.7.31
%define meego_ver 0.1.0
%define meego_ver1 0.1.0.1

Summary: Statefs providers
Name: statefs-providers
Version: x.x.x
Release: 1
License: LGPLv2
Group: System Environment/Libraries
URL: http://github.com/nemomobile/statefs-providers
Source0: %{name}-%{version}.tar.bz2
BuildRequires: cmake >= 2.8
BuildRequires: pkgconfig(statefs-cpp) >= 0.3.8
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5DBus)

%description
%{summary}

%define p_common -n statefs-providers-common

%package %{p_common}
Summary: Package to replace contextkit plugins
Group: Applications/System
Requires: statefs >= 0.3.8
Obsoletes: contextkit-maemo <= %{maemo_ver}
Provides: contextkit-maemo = %{maemo_ver1}
Obsoletes: contextkit-meego <= %{meego_ver}
Provides: contextkit-meego = %{meego_ver1}
Obsoletes: statefs-contextkit-provider <= %{ckit_statefs_version}
Provides: statefs-contextkit-provider = %{ckit_statefs_version}
%description %{p_common}
%{summary}

%package -n statefs-qt5
Summary: StateFS Qt5 bindings
Group: System Environment/Libraries
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: statefs >= 0.3.6
%description -n statefs-qt5
%{summary}

%package -n statefs-qt5-devel
Summary: StateFS Qt5 bindings development files
Group: Development/Libraries
Requires: statefs-qt5 = %{version}
%description -n statefs-qt5-devel
%{summary}

%package -n statefs-qt-doc
Summary: StateFS Qt bindings documentation
Group: Documenation
BuildRequires: doxygen
%if 0%{?_with_docs:1}
BuildRequires: graphviz
%endif
%description -n statefs-qt-doc
%{summary}

%define p_bluez -n statefs-provider-bluez
%define p_upower -n statefs-provider-upower

%package %{p_bluez}
Summary: BlueZ statefs provider
Group: Applications/System
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Requires: statefs-providers-common = %{version}
Requires: statefs-loader-qt5
Obsoletes: contextkit-plugin-bluez <= %{ckit_version}
Provides: contextkit-plugin-bluez = %{ckit_version}
Obsoletes: contextkit-plugin-bluetooth <= %{ckit_version}
Provides: contextkit-plugin-bluetooth = %{ckit_version}
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
Provides: contextkit-plugin-upower = %{ckit_version}
Provides: statefs-provider-power = %{version}
%description %{p_upower}
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

install -d -D -p -m755 %{buildroot}%{_datarootdir}/doc/statefs-qt/html
cp -R doc/html/ %{buildroot}%{_datarootdir}/doc/statefs-qt/

%clean
rm -rf %{buildroot}

%files %{p_common}
%defattr(-,root,root,-)
%doc README

%files -n statefs-qt5
%defattr(-,root,root,-)
%{_libdir}/libstatefs-qt5.so

%post -n statefs-qt5 -p /sbin/ldconfig
%postun -n statefs-qt5 -p /sbin/ldconfig

%files -n statefs-qt5-devel
%defattr(-,root,root,-)
%{_qt5_headerdir}/statefs/qt/*.hpp
%{_libdir}/pkgconfig/statefs-qt5.pc

%files -n statefs-qt-doc
%defattr(-,root,root,-)
%{_datarootdir}/doc/statefs-qt/html/*

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
