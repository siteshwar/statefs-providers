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
    , current_net_order_(OrderEnd)
    , net_type_map_{
    {"wifi", "WLAN"}
    , {"gprs", "GPRS"}
    , {"cellular", "GPRS"}
    , {"edge", "GPRS"}
    , {"umts", "GPRS"}
    , {"ethernet", "ethernet"}}
    , state_map_{
        {"offline", "disconnected"}
        , {"idle", "disconnected"}
        , {"online", "connected"}
        , {"ready", "connected"}}
{
}

void Bridge::process_manager_props(QVariantMap const &props)
{

    auto update = [this](QString const &n, QVariant const &v) {
        if (n == "State") {
            if (state_map_[v.toString()] == "connected")
                process_technologies();
            else
                reset_properties();
        }
    };
    connect(manager_.get(), &Manager::PropertyChanged
            , [update] (QString const &n, QDBusVariant const &v) {
                update(n, v.variant());
            });

    for (auto it = props.begin(); it != props.end(); ++it)
        update(it.key(), it.value());

    connect(manager_.get(), &Manager::TechnologyAdded
            , [this] (const QDBusObjectPath &path, const QVariantMap &props) {
                process_technology(path.path(), props);
            });
    connect(manager_.get(), &Manager::TechnologyRemoved
            , [this] (const QDBusObjectPath &path) {
                if (path.path() == current_technology_)
                    process_technologies();
            });
    connect(manager_.get(), &Manager::ServicesAdded
            , [this] (PathPropertiesArray const &info) {
                process_services();
            });
    connect(manager_.get(), &Manager::ServicesRemoved
            , [this] (const QList<QDBusObjectPath> &data) {
                auto services = QSet<QDBusObjectPath>::fromList(data);
                if (services.contains(QDBusObjectPath(current_service_)))
                    process_technologies();
            });
}

void Bridge::init()
{
    auto init_manager = [this]() {
        qDebug() << "Establish connection with connman";
        manager_.reset(new Manager(service_name, "/", bus_));
        sync(manager_->GetProperties()
             , [this](QVariantMap const &v) {
                 process_manager_props(v);
             });
    };
    watch_->init(init_manager, [this]() { reset_manager(); });
    init_manager();
}

void Bridge::reset_properties()
{
    qDebug() << "Internet: reset properties";
    static_cast<InternetNs*>(target_)->reset_properties();
}

Bridge::Order Bridge::get_order(QString const &net_type)
{
    return (net_type == "wifi"
            ? WiFi : (net_type == "cellular"
                      ? Cellular : Other));
}

void Bridge::reset_manager()
{
    qDebug() << "Connman is unregistered, cleaning up";
    manager_.reset();
    reset_properties();
}

void Bridge::process_technologies()
{
    current_net_order_ = OrderEnd;
    current_technology_ = "";
    current_service_ = "";
    service_.reset();

    auto process_props = [this](PathPropertiesArray const &techs) {
        for (auto pp = techs.begin(); pp != techs.end(); ++pp) {
            auto const &path = std::get<0>(*pp).path();
            auto const &props = std::get<1>(*pp);
            process_technology(path, props);
        }
        if (current_net_order_ == OrderEnd)
            reset_properties();
    };

    sync(manager_->GetTechnologies(), process_props);
    if (current_service_ == "")
        process_services();
}

Bridge::Status Bridge::process_service
(QString const &path, QVariantMap const &props)
{
    auto order = service_order(props);

    if (order > current_net_order_)
        return Ignore;

    auto name = props["Name"].toString();
    qDebug() << "Service " << name << " is online";

    current_net_order_ = order;
    current_service_ = path;

    updateProperty("NetworkName", name);
    updateProperty("SignalStrength", props["Strength"].toUInt());
    auto state = state_map_[props["State"].toString()];
    updateProperty("NetworkState", state);
    auto net_type = net_type_map_[props["Type"].toString()];
    updateProperty("NetworkType", net_type);

    service_.reset(new Service(service_name, path, bus_));

    auto update = [this](QString const &n, QVariant const &v) {
        if (n == "Name")
            updateProperty("NetworkName", v);
        else if (n == "Strength")
            updateProperty("SignalStrength", v.toUInt());
        else if (n == "State") {
            auto state = state_map_[v.toString()];
            if (state != "connected")
                process_technologies();
        }
    };

    connect(service_.get(), &Service::PropertyChanged
            , [update](QString const &n, QDBusVariant const&v) {
                update(n, v.variant());
            });
    return Match;
}

Bridge::Order Bridge::service_order(QVariantMap const &props)
{
    auto service_type = props["Type"].toString();
    auto state = state_map_[props["State"].toString()];
    auto order = get_order(service_type);

    return (state == "connected") ? order : OrderEnd;
}

Bridge::Status Bridge::process_services()
{
    Status status = Ignore;
    auto process = [this, &status]
        (PathPropertiesArray const &services) {

        decltype(services.begin()) chosen;
        QString chosen_path;
        QVariantMap const *chosen_props;
        Order chosen_order = OrderEnd;

        for (auto pp = services.begin(); pp != services.end(); ++pp) {
            auto const &path = std::get<0>(*pp).path();
            auto const &props = std::get<1>(*pp);
            auto order = service_order(props);
            if (order < chosen_order) {
                chosen_path = path;
                chosen_props = &props;
                chosen_order = order;
            }
        }
        if (chosen_order != OrderEnd)
            status = process_service(chosen_path, *chosen_props);
    };
    sync(manager_->GetServices(), process);
    return status;
}

Bridge::Status Bridge::process_technology(QString const &path
                                          , QVariantMap const &props)
{
    auto net_type = props["Type"].toString();
    auto is_connected = props["Connected"].toBool();

    if (!is_connected)
        return Ignore;

    auto order = get_order(net_type);
    if (order > current_net_order_)
        return Ignore;

    qDebug() << "Technology (type=" << net_type << ") is online";
    updateProperty("NetworkType", net_type_map_[net_type]);
    updateProperty("NetworkState", "connected");
    current_net_order_ = order;
    current_technology_ = path;
    auto status = process_services();
    return (order == WiFi || order == Cellular) ? status : Match;
}

InternetNs::InternetNs(QDBusConnection &bus)
    : Namespace("Internet", std::unique_ptr<PropertiesSource>
                (new Bridge(this, bus)))
    , defaults_({{"NetworkType", ""}
            , {"NetworkState", "offline"}
            , {"NetworkName", ""}
            //, {"TrafficIn", "0"}
            //, {"TrafficOut", "0"}
            , {"SignalStrength", "0"}})
{
    addProperty("NetworkType", "");
    addProperty("NetworkState", "disconnected");
    addProperty("NetworkName", "");
    addProperty("TrafficIn", "0");
    addProperty("TrafficOut", "0");
    addProperty("SignalStrength", "0");
    src_->init();
}

void InternetNs::reset_properties()
{
    setProperties(defaults_);
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
