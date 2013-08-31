#ifndef _STATEFS_PRIVATE_BLUEZ_HPP_
#define _STATEFS_PRIVATE_BLUEZ_HPP_

#include "manager_interface.h"
#include "device_interface.h"

#include <statefs/provider.hpp>
#include <statefs/property.hpp>
#include <statefs/qt/ns.hpp>

#include <QObject>

namespace statefs { namespace upower {

typedef OrgFreedesktopUPowerInterface Manager;
typedef OrgFreedesktopUPowerDeviceInterface Device;

class PowerNs;

class Bridge : public QObject, public statefs::qt::PropertiesSource
{
    Q_OBJECT;
public:

    enum DeviceState {
        UnknownState = 0,
        Charging,
        Discharging,
        Empty,
        FullyCharged,
        PendingCharge,
        PendingDischarge
    };

    enum DeviceType {
        UnknownDevice,
        LinePower,
        Battery,
        Ups,
        Monitor,
        Mouse,
        Keyboard,
        Pda,
        Phone
    };

    Bridge(PowerNs *, QDBusConnection &bus);

    virtual ~Bridge() {}

    virtual void init();

private:

    void initManager();
    void updateAllProperties();
    bool findBattery();
    void watchUPower();

    QDBusConnection &bus_;
    QDBusObjectPath defaultAdapter_;
    std::unique_ptr<QDBusServiceWatcher> watcher_;
    std::unique_ptr<Manager> manager_;
    std::unique_ptr<Device> device_;
};

class PowerNs : public statefs::qt::Namespace
{
public:

    PowerNs(QDBusConnection &bus);
};

}}

#endif // _STATEFS_PRIVATE_BLUEZ_HPP_
