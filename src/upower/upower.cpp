/*
 * StateFS UPower provider
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

#include "upower.hpp"
#include <math.h>
#include <iostream>
#include <statefs/qt/dbus.hpp>


namespace statefs { namespace upower {

using statefs::qt::Namespace;
using statefs::qt::PropertiesSource;
using statefs::qt::sync;

static char const *service_name = "org.freedesktop.UPower"; 

Bridge::Bridge(PowerNs *ns, QDBusConnection &bus)
    : PropertiesSource(ns)
    , bus_(bus)
{
}

void Bridge::updateAllProperties()
{
    using namespace std::placeholders;
    auto set = std::bind(&PropertiesSource::updateProperty, this, _1, _2);
    auto c = device_->capacity();
    set("ChargePercentage", round(c));
    set("Capacity", c);
    set("OnBattery", manager_->onBattery());
    set("LowBattery", manager_->onLowBattery());
    set("TimeUntilLow", device_->timeToEmpty());
    set("TimeUntilFull", device_->timeToFull());
    set("Energy", device_->energy());
    set("EnergyFull", device_->energyFull());
    DeviceState state = (DeviceState)device_->state();
    set("IsCharging", state == Charging || state == FullyCharged);
}

bool Bridge::findBattery()
{
    qDebug() << "Get devices";
    auto devices = sync(manager_->EnumerateDevices()).value();
    for (auto it = devices.begin(); it != devices.end(); ++it) {
		std::unique_ptr<Device> device(new Device(service_name, it->path(), bus_));
		auto dev_type = (DeviceType)device->type();

        qDebug() << "UPower dev " << dev_type << ", E:" << device->energyFull()
                 << " V:" << device->voltage()
                 << " = " << device->nativePath();
		if(dev_type == Battery && device->energyFull() > 0 && device->voltage() > 0)
		{
            qDebug() << " is battery";
			device_ = std::move(device);
            updateAllProperties();
            connect(device_.get(), &Device::Changed
                    , this, &Bridge::updateAllProperties);
			return true;
		}
    }
    qWarning() << "No battery found";
    return false;
}

void Bridge::watchUPower()
{
    if (watcher_)
        return;

    watcher_.reset
        (new QDBusServiceWatcher(service_name, bus_));
    connect(watcher_.get(), &QDBusServiceWatcher::serviceRegistered
            , [this]() {
                qDebug() << "Got UPower";
                init();
            });
    connect(watcher_.get(), &QDBusServiceWatcher::serviceUnregistered
            , [this]() {
                qDebug() << "Lost UPower";
                device_.reset();
                manager_.reset();
            });
    connect(watcher_.get(), &QDBusServiceWatcher::serviceOwnerChanged
            , [this](const QString &serviceName, const QString &oldOwner, const QString &newOwner) {
                qDebug() << serviceName << " owner is changed " << oldOwner << "->" << newOwner;
            });
}

void Bridge::init()
{
    manager_.reset(new Manager(service_name, "/org/freedesktop/UPower", bus_));
    if (!findBattery())
        watchUPower();
}

PowerNs::PowerNs(QDBusConnection &bus)
    : Namespace("Battery", std::unique_ptr<PropertiesSource>
                (new Bridge(this, bus)))
{
    addProperty("ChargePercentage", "87");
    addProperty("Capacity", "87");
    addProperty("OnBattery", "1");
    addProperty("LowBattery", "0");
    addProperty("TimeUntilLow", "878787");
    addProperty("TimeUntilFull", "0");
    addProperty("IsCharging", "0");
    addProperty("Energy", "7");
    addProperty("EnergyFull", "8");
    src_->init();
}

class Provider;
static Provider *provider = nullptr;

class Provider : public statefs::AProvider
{
public:
    Provider(statefs_server *server)
        : AProvider("upower", server)
        , bus_(QDBusConnection::systemBus())
    {
        auto ns = std::make_shared<PowerNs>(bus_);
        insert(std::static_pointer_cast<statefs::ANode>(ns));
    }
    virtual ~Provider() {}

    virtual void release() {
        if (this == provider) {
            delete provider;
            provider = nullptr;
        }
    }

private:
    QDBusConnection bus_;
};

static inline Provider *init_provider(statefs_server *server)
{
    if (provider)
        throw std::logic_error("provider ptr is already set");
    provider = new Provider(server);
    return provider;
}

}}

EXTERN_C struct statefs_provider * statefs_provider_get
(struct statefs_server *server)
{
    return statefs::upower::init_provider(server);
}
