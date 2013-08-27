#ifndef _STATEFS_PRIVATE_BLUEZ_HPP_
#define _STATEFS_PRIVATE_BLUEZ_HPP_

#include "manager_interface.h"
#include "adapter_interface.h"

#include <statefs/provider.hpp>
#include <statefs/property.hpp>
#include <statefs/qt/ns.hpp>

#include <QObject>

namespace statefs { namespace bluez {

typedef OrgBluezManagerInterface Manager;
typedef OrgBluezAdapterInterface Device;

class BlueZ;

class Bridge : public QObject, public statefs::qt::PropertiesSource
{
    Q_OBJECT;
public:

    Bridge(BlueZ *, QDBusConnection &);

    virtual ~Bridge() {}

    virtual void init();

private slots:
    void defaultAdapterChanged(const QDBusObjectPath &);
    void propertyChanged(const QString &, const QDBusVariant &);

private:

    bool createDevice(const QDBusObjectPath &);

    QDBusConnection &bus_;
    QDBusObjectPath defaultAdapter_;
    std::unique_ptr<Manager> manager_;
    std::unique_ptr<Device> device_;
};

class BlueZ : public statefs::qt::Namespace
{
public:

    BlueZ(QDBusConnection &bus);
};

}}

#endif // _STATEFS_PRIVATE_BLUEZ_HPP_
