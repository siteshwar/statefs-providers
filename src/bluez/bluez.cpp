/*
 * StateFS BlueZ provider
 *
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Denis Zalevskiy <denis.zalevskiy@jollamobile.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
 */

#include "bluez.hpp"
#include <iostream>
#include <statefs/qt/dbus.hpp>

/*
QDBusArgument & operator <<(QDBusArgument &argument, BluezService const& src)
{
    argument.beginStructure();
    argument << src.i << src.s;
    argument.endStructure();
    return argument;
}

QDBusArgument const& operator >> (QDBusArgument const& argument, BluezService &dst)
{
    argument.beginStructure();
    argument >> dst.i >> dst.s;
    argument.endStructure();
    return argument;
}
*/

namespace statefs { namespace bluez {

using statefs::qt::Namespace;
using statefs::qt::PropertiesSource;
using statefs::qt::sync;


Bridge::Bridge(BlueZ *ns, QDBusConnection &bus)
    : PropertiesSource(ns)
    , bus_(bus)
    , manager_(new Manager("org.bluez", "/", bus))
{
}

void Bridge::init()
{
    connect(manager_.get(), &Manager::DefaultAdapterChanged
            , this, &Bridge::defaultAdapterChanged);

    auto getDefault = sync(manager_->DefaultAdapter());
    if (getDefault.isError()) {
        qWarning() << "DefaultAdapter error:" << getDefault.error();
    } else {
        defaultAdapterChanged(getDefault.value());
    }
}

void Bridge::defaultAdapterChanged(const QDBusObjectPath &v)
{
    qDebug() << "New default bluetooth adapter" << v.path();
    defaultAdapter_ = v;

    if (!createDevice(v))
        return;

    auto res = sync(device_->GetProperties());
    auto props = res.value();
    setProperties(props);

    connect(device_.get(), &Device::PropertyChanged
            , [this](const QString &name, const QDBusVariant &value) {
                updateProperty(name, value.variant());
            });
}

BlueZ::BlueZ(QDBusConnection &bus)
    : Namespace("Bluetooth", std::unique_ptr<PropertiesSource>
                (new Bridge(this, bus)))
{
    addProperty("Enabled", "0", "Powered");
    addProperty("Visible", "0", "Discoverable");
    addProperty("Connected", "0", "Connected");
    addProperty("Address", "00:00:00:00:00:00", "Address");
    src_->init();
}

bool Bridge::createDevice(const QDBusObjectPath &v)
{
    device_.reset(new Device("org.bluez", v.path(), bus_));
    if (!device_) {
        qWarning() << "Can't create dbus interface";
        return false;
    }
    return true;
}

class Provider : public statefs::AProvider
{
public:
    Provider(statefs_server *server)
        : AProvider("bluez", server)
        , bus_(QDBusConnection::systemBus())
    {
        auto ns = std::make_shared<BlueZ>(bus_);
        insert(std::static_pointer_cast<statefs::ANode>(ns));
    }
    virtual ~Provider() {}

    virtual void release() {
        delete this;
    }

private:
    QDBusConnection bus_;
};

static Provider *provider = nullptr;

static inline Provider *init_provider(statefs_server *server)
{
    if (provider)
        throw std::logic_error("provider ptr is already set");
    registerDataTypes();
    provider = new Provider(server);
    return provider;
}

}}

EXTERN_C struct statefs_provider * statefs_provider_get
(struct statefs_server *server)
{
    return statefs::bluez::init_provider(server);
}
