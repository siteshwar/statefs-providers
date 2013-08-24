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

class BlueZ : public statefs::Namespace, public QObject
{
    Q_OBJECT;
public:

    BlueZ(QDBusConnection &bus);

    virtual ~BlueZ() {}
    virtual void release() { }
private slots:
    // void adapterAdded(const QDBusObjectPath &);
    // void adapterRemoved(const QDBusObjectPath &);
    void defaultAdapterChanged(const QDBusObjectPath &);

private:
    QDBusConnection &bus_;
    std::unique_ptr<Manager> manager_;
    std::unique_ptr<Device> device_;
    // setter_type set_is_enabled_;
    // setter_type set_is_visible_;
    // setter_type set_is_connected_;
    QMap<QString, QDBusObjectPath> adapters_;
    QDBusObjectPath defaultAdapter_;
    QMap<QString, setter_type> setters_for_props_;
};

}}

#endif // _STATEFS_PRIVATE_BLUEZ_HPP_
