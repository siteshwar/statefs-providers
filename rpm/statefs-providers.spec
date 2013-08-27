%define ckit_version 0.7.41
%define ckit_version 0.7.41
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
Obsoletes: statefs-contextkit-provider <= %{ckit_version}
Provides: statefs-contextkit-provider = %{ckit_version}
%description %{p_common}
%{summary}

%define p_bluez -n statefs-provider-bluez
%define p_upower -n statefs-provider-upower

%package %{p_bluez}
Summary: BlueZ statefs provider
Group: Applications/System
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

%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}

%clean
rm -rf %{buildroot}

%files %{p_common}
%defattr(-,root,root,-)
%doc README

%files %{p_bluez}
%defattr(-,root,root,-)
%{_libdir}/statefs/libprovider-bluez.so

%post %{p_bluez}
statefs register %{_libdir}/statefs/libprovider-bluez.so --statefs-type=qt5 || :
