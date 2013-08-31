/*
 * StateFS Connman provider
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

#include "provider_connman.hpp"
#include <math.h>
#include <iostream>
#include <statefs/qt/dbus.hpp>
#include "dbus_types.hpp"

namespace statefs { namespace connman {

using statefs::qt::Namespace;
using statefs::qt::PropertiesSource;
using statefs::qt::sync;

static char const *service_name = "net.connman";

Bridge::Bridge(InternetNs *ns, QDBusConnection &bus)
    : PropertiesSource(ns)
    , bus_(bus)
    , watch_(new ServiceWatch(bus, service_name))
    , defaults_{{"NetworkType", ""}
                           , {"NetworkState", "offline"}
                           , {"NetworkName", ""}
                           // , {"TrafficIn", "0"}
                           // , {"TrafficOut", "0"}
                           , {"SignalStrength", "0"}}
{
}

void Bridge::init()
{
    qDebug() << "Establish connection with connman";
    watch_->init([this]() { init(); }, [this]() { resetManager(); });
    manager_.reset(new Manager(service_name, "/", bus_));
    auto props = sync(manager_->GetProperties());
    if (props.isError()) {
        qWarning() << "Can't get connman props:" << props.error();
        return;
    }
    auto updateState = [this](QVariant const &v) {
        updateProperty("NetworkState", v);
        if (v == "online")
            processTechnologies();
        else
            setProperties(defaults_);
    };
    connect(manager_.get(), &Manager::PropertyChanged
            , [this, updateState] (QString const &n, QDBusVariant const &v) {
                if (n == "State")
                    updateState(v.variant());
            });
    auto v = props.value();
    updateState(v["State"]);
}

void Bridge::resetManager()
{
    qDebug() << "Connman is unregistered, cleaning up";
    manager_.reset();
    setProperties(defaults_);
}

void Bridge::processTechnologies()
{
    auto technologies = sync(manager_->GetTechnologies());
    if (technologies.isError()) {
        qWarning() << "Can't get connman technologies:" << technologies.error();
        return;
    }
    auto v = technologies.value();
    for (auto pp = v.begin(); pp != v.end(); ++pp) {
        if (processTechnology(std::get<1>(*pp)) == ExactMatch)
            break;
    }
}

Bridge::Status Bridge::processService(QVariantMap const &props)
{
    auto service_type = props["Type"];
    if (props["Type"] == "wifi" && props["State"] == "online") {
        auto name = props["Name"];
        qDebug() << "Wifi" << name.toString() << " is online";
        updateProperty("NetworkName", name);
        updateProperty("SignalStrength", props["Strength"]);
        return Match;
    }
    return Ignore;
}

Bridge::Status Bridge::processServices()
{
    auto services = sync(manager_->GetServices());
    if (services.isError()) {
        qWarning() << "Can't get connman services:" << services.error();
        return Ignore;
    }
    auto v = services.value();
    for (auto pp = v.begin(); pp != v.end(); ++pp) {
        if (processService(std::get<1>(*pp)) != Ignore)
            return ExactMatch;
    }
    return Ignore;
}

Bridge::Status Bridge::processTechnology(QVariantMap const &props)
{
    auto net_type = props["Type"].toString();
    auto is_connected = props["Connected"].toBool();
    if (!is_connected)
        return Ignore;

    updateProperty("NetworkType", net_type);

    if (net_type == "wifi")
        return processServices();

    qDebug() << "Net (type=" << net_type << ") is online";
    return Match;
}

InternetNs::InternetNs(QDBusConnection &bus)
    : Namespace("Internet", std::unique_ptr<PropertiesSource>
                (new Bridge(this, bus)))
{
    addProperty("NetworkType", "");
    addProperty("NetworkState", "offline");
    addProperty("NetworkName", "");
    addProperty("TrafficIn", "0");
    addProperty("TrafficOut", "0");
    addProperty("SignalStrength", "0");
    src_->init();
}

class Provider;
static Provider *provider = nullptr;

class Provider : public statefs::AProvider
{
public:
    Provider(statefs_server *server)
        : AProvider("connman", server)
        , bus_(QDBusConnection::systemBus())
    {
        auto ns = std::make_shared<InternetNs>(bus_);
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
    registerDataTypes();
    if (provider)
        throw std::logic_error("provider ptr is already set");
    provider = new Provider(server);
    return provider;
}

}}

EXTERN_C struct statefs_provider * statefs_provider_get
(struct statefs_server *server)
{
    return statefs::connman::init_provider(server);
}
