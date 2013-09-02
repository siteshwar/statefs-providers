/*
 * StateFS oFono provider
 *
 * Properties translation from ofono values is performed in
 * contextkit-compatible way.
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

#include "provider_ofono.hpp"
#include <math.h>
#include <iostream>
#include <statefs/qt/dbus.hpp>
#include "dbus_types.hpp"

namespace statefs { namespace ofono {

using statefs::qt::Namespace;
using statefs::qt::PropertiesSource;
using statefs::qt::sync;

static char const *service_name = "org.ofono";

Bridge::Bridge(MainNs *ns, QDBusConnection &bus)
    : PropertiesSource(ns)
    , bus_(bus)
    , watch_(bus, service_name)
    , has_sim_(false)
    , tech_map_{
    {"gsm", {"gsm", "gprs"}}
    , {"edge", {"gsm", "egprs"}}
    , {"hspa", {"umts", "hspa"}}
    , {"umts", {"umts", "umts"}}
    , {"lte", {"lte", "lte"}}}
    , status_map_{
        {"registered", "home"}
        , {"roaming", "roam"}
        , {"denied", "forbidden"}}
    , net_property_actions_({
        { "Name", [this](QVariant const &v) {
                updateProperty("NetworkName", v);
                updateProperty("ExtendedNetworkName", v);
            } }
        , { "Strength", [this](QVariant const &v) {
                auto strength = v.toUInt();
                updateProperty("SignalStrength", strength);
                // 0-5
                updateProperty("SignalBars", (strength + 19) / 20);
            } }
        , { "Status", [this](QVariant const &v) {
                if (!sim_)
                    return;
                auto status = v.toString();
                auto it = status_map_.find(status);
                status = (it != status_map_.end())
                    ? it->second : QString("offline");
                updateProperty("RegistrationStatus", status);
                updateProperty("Status", v);
            } }
        , { "CellId", [this](QVariant const &v) {
                updateProperty("CellName", v);
            } }
        , { "Technology", [this](QVariant const &v) {
                auto tech = v.toString();
                auto pt = tech_map_.find(tech);
                if (pt != tech_map_.end()) {
                    auto tech_dtech = pt->second;
                    updateProperty("Technology", tech_dtech.first);
                    updateProperty("DataTechnology", tech_dtech.second);
                }
            } }
    })
    , sim_props_map_({
            {"MobileCountryCode", "CountryCode"}
            , {"MobileNetworkCode", "NetworkCode"}})
{
}

void Bridge::reset_modem()
{
    qDebug() << "Resetting mode properties";
    modem_.reset();
    reset_sim();
}

void Bridge::reset_sim()
{
    qDebug() << "Resetting sim properties";
    sim_.reset();
    network_.reset();
    has_sim_ = false;
    static_cast<MainNs*>(target_)->resetProperties(MainNs::NoSimDefault);
}

void Bridge::reset_network()
{
    qDebug() << "Resetting network properties";
    network_.reset();
    static_cast<MainNs*>(target_)->resetProperties(MainNs::Default);
}

void Bridge::process_features(QStringList const &v)
{
    auto features = QSet<QString>::fromList(v);
    qDebug() << "Features: " << features;
    bool has_sim_feature = features.contains("sim");
    if (sim_) {
        if (!has_sim_feature)
            reset_sim();
    } else if (has_sim_feature) {
        setup_sim(modem_path_);
    }
    bool has_net = features.contains("net");
    if (network_) {
        if (has_sim_feature && !has_net)
            reset_network();
    } else if (has_net && has_sim_) {
        setup_network(modem_path_);
    }
}

bool Bridge::setup_modem(QString const &path, QVariantMap const &props)
{
    if (props["Type"].toString() != "hardware") {
        // TODO hardcoded for phones
        return false;
    }
    qDebug() << "HW modem " << path;

    modem_.reset(new Modem(service_name, path, bus_));
    modem_path_ = path;
    // auto set_homonym = [this, &props](QString const &name) {
    //     updateProperty(name, props[name]);
    // };
    // set_homonym("Manufacturer");
    // set_homonym("Model");
    // set_homonym("Serial");
    // set_homonym("Revision");

    connect(modem_.get(), &Modem::PropertyChanged
            , [this](QString const &n, QDBusVariant const &v) {
                if (n == "Features")
                    process_features(v.variant().toStringList());
            });
    process_features(props["Features"].toStringList());
    return true;
}

void Bridge::init()
{
    qDebug() << "Establish connection with ofono";

    auto connect_manager = [this]() {
        manager_.reset(new Manager(service_name, "/", bus_));
        auto res = sync(manager_->GetModems());
        if (res.isError()) {
            qWarning() << "GetModems error:" << res.error();
            return;
        }
        auto modems = res.value();
        qDebug() << "There is(are) " << modems.size() << " modems";
        if (!modems.size())
            return;

        for (auto it = modems.begin(); it != modems.end(); ++it) {
            auto const &info = *it;
            auto path = std::get<0>(info).path();
            auto props = std::get<1>(info);

            if (setup_modem(path, props))
                break;
        }
        connect(manager_.get(), &Manager::ModemAdded
                , [this](QDBusObjectPath const &n, QVariantMap const&p) {
                    setup_modem(n.path(), p);
                });
        connect(manager_.get(), &Manager::ModemRemoved
                , [this](QDBusObjectPath const &n) {
                    if (n.path() == modem_path_)
                        reset_modem();
                });
    };
    auto reset_manager = [this]() {
        qDebug() << "Ofono is unregistered, cleaning up";
        manager_.reset();
        static_cast<MainNs*>(target_)->resetProperties(MainNs::Default);
    };
    watch_.init(connect_manager, reset_manager);
    connect_manager();
}

void Bridge::setup_network(QString const &path)
{
    qDebug() << "Getting network properties";
    auto update = [this](QString const &n, QVariant const &v) {
        auto paction = net_property_actions_.find(n);
        if (paction != net_property_actions_.end()) {
            auto action = paction->second;
            action(v);
        }
    };
    network_.reset(new Network(service_name, path, bus_));
    auto res = sync(network_->GetProperties());
    if (res.isError()) {
        qWarning() << "Network GetProperties error:" << res.error();
        return;
    }
    auto props = res.value();
    for (auto it = props.begin(); it != props.end(); ++it)
        update(it.key(), it.value());
    connect(network_.get(), &Network::PropertyChanged
            , [update](QString const &n, QDBusVariant const &v) {
                update(n, v.variant());
            });
}

void Bridge::setup_sim(QString const &path)
{
    qDebug() << "Getting sim properties";
    auto update = [this](QString const &n, QVariant const &v) {
        if (n == "Present") {
            has_sim_ = v.toBool();
            if (has_sim_) {
                if (!network_)
                    updateProperty("RegistrationStatus", "offline");
            } else {
                reset_sim();
            }
        } else {
            auto it = sim_props_map_.find(n);
            if (it != sim_props_map_.end())
                updateProperty(it->second, v);
        }
        // auto paction = net_property_actions_.find(n);
        // if (paction != net_property_actions_.end()) {
        //     auto action = paction->second;
        //     action(v);
        // }
    };
    sim_.reset(new SimManager(service_name, path, bus_));
    auto res = sync(sim_->GetProperties());
    if (res.isError()) {
        qWarning() << "Sim GetProperties error:" << res.error();
        return;
    }
    auto props = res.value();
    for (auto it = props.begin(); it != props.end(); ++it)
        update(it.key(), it.value());
    connect(sim_.get(), &SimManager::PropertyChanged
            , [update](QString const &n, QDBusVariant const &v) {
                update(n, v.variant());
            });
}

void MainNs::resetProperties(MainNs::Properties what)
{
    setProperties(defaults_);
    if (what == NoSimDefault)
        updateProperty("RegistrationStatus", "no-sim");
}


MainNs::MainNs(QDBusConnection &bus)
    : Namespace("Cellular", std::unique_ptr<PropertiesSource>
                (new Bridge(this, bus)))
    , defaults_({
            { "SignalStrength", "0"}
            , { "DataTechnology", "unknown"}
            , { "RegistrationStatus", "offline"} // contextkit
            , { "NetworkStatus", "offline"} // ofono
            , { "Technology", "unknown"}
            , { "SignalBars", "0"}
            , { "CellName", ""}
            , { "NetworkName", ""}
            , { "ExtendedNetworkName", "" }
            , { "CountryCode", "0"}
            , { "NetworkCode", "0"}})
{
    for (auto v : defaults_)
        addProperty(v.first, v.second);
    src_->init();
}

class Provider;
static Provider *provider = nullptr;

class Provider : public statefs::AProvider
{
public:
    Provider(statefs_server *server)
        : AProvider("ofono", server)
        , bus_(QDBusConnection::systemBus())
    {
        auto ns = std::make_shared<MainNs>(bus_);
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
    return statefs::ofono::init_provider(server);
}
