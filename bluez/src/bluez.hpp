#ifndef _STATEFS_PRIVATE_BLUEZ_HPP_
#define _STATEFS_PRIVATE_BLUEZ_HPP_

#include "manager.h"
#include "device.h"

#include <statefs/provider.hpp>
#include <statefs/property.hpp>

#include <QObject>

namespace statefs { namespace bluez {

typedef OrgBluezManagerInterface Manager;
typedef OrgBluezDeviceInterface Device;

class BlueZ;

class Bridge : public QObject
{
    Q_OBJECT;
public:

    Bridge(QDBusConnection &bus);

    virtual ~Bridge() {}

private:

    friend class BlueZ;

    void createDevice(const QDBusObjectPath &);

    void defaultAdapterChanged(const QDBusObjectPath &);

    QDBusConnection &bus_;
    std::unique_ptr<Manager> manager_;
    std::unique_ptr<Device> device_;
};

class BlueZ : public statefs::Namespace
{
public:

    BlueZ(QDBusConnection &bus);

    virtual ~BlueZ() {}
    virtual void release() { }

private:

    void defaultAdapterChanged(const QDBusObjectPath &);

    QDBusObjectPath defaultAdapter_;
    QMap<QString, setter_type> setters_for_props_;
    Bridge bridge_;
    //QDBusConnection &bus_;
    // setter_type set_is_enabled_;
    // setter_type set_is_visible_;
    // setter_type set_is_connected_;
    //QMap<QString, QDBusObjectPath> adapters_;
};

}}

#endif // _STATEFS_PRIVATE_BLUEZ_HPP_
