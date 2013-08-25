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


namespace statefs { namespace bluez {

template <typename T>
inline QDBusPendingReply<T> sync(QDBusPendingReply<T> &&reply)
{
    QDBusPendingCallWatcher watcher(reply);
    watcher.waitForFinished();
    return reply;
}

Bridge::Bridge(QDBusConnection &bus)
    : bus_(bus)
    , manager_(new Manager("org.bluez", "/", bus))
{}

BlueZ::BlueZ(QDBusConnection &bus)
    : statefs::Namespace("Bluetooth")
    , bridge_(bus)
{
    auto addProp = [this](char const *name
                          , char const *defVal
                          , char const *bluezName) {
        using statefs::Discrete;
        auto d = Discrete(name, defVal);
        auto prop = d.create();
        *this << prop;
        setters_for_props_[bluezName] = setter(prop);
    };
    addProp("Enabled", "0", "Powered");
    addProp("Visible", "0", "Discoverable");
    addProp("Connected", "0", "Connected");

    // auto adapterAdd = [this](const QDBusObjectPath &v) {
    //     adapters_[v.path()] = v;
    // };
    // auto adapterRm = [this](const QDBusObjectPath &v) {
    //     adapters_.remove(v.path());
    // };
    // connect(manager_.get(), &Manager::AdapterAdded, adapterAdd);
    // connect(manager_.get(), &Manager::AdapterRemoved, adapterRm);
    bridge_.connect(bridge_.manager_.get()
                    , &Manager::DefaultAdapterChanged
                    , [this](QDBusObjectPath const &p) {
                        defaultAdapterChanged(p);
                    });

    // auto adapters = sync(bridge_.manager_->ListAdapters());
    // if (adapters.isError()) {
    //     qWarning() << "ListAdapters error:" << adapters.error();
    // } else {
    //     for (auto v : adapters.value())
    //         adapterAdd(v);
    // }

    auto getDefault = sync(bridge_.manager_->DefaultAdapter());
    if (getDefault.isError()) {
        qWarning() << "DefaultAdapter error:" << getDefault.error();
    } else {
        defaultAdapterChanged(getDefault.value());
    }
}

void Bridge::createDevice(const QDBusObjectPath &v)
{
    device_.reset(new Device("org.bluez.Device", v.path(), bus_));
}

void BlueZ::defaultAdapterChanged(const QDBusObjectPath &v)
{
    qDebug() << "DefaultAdapter" << v.path();
    defaultAdapter_ = v;

    bridge_.createDevice(v);
    auto strFromVarBool = [](QVariant const &v, bool cond1 = true) {
        return (cond1 && v.type() == QVariant::Bool && v.toBool()) ? "1" : "0";
    };
    auto boolProp = [strFromVarBool](QVariantMap const &props, QString const &name) {
        auto it = props.find(name);
        return strFromVarBool(it.value(), it != props.end());
    };

    auto res = sync(bridge_.device_->GetProperties());
    auto props = res.value();
    for(auto it = setters_for_props_.begin();
        it != setters_for_props_.end(); ++it) {
        it.value()(boolProp(props, it.key()));
    }

    auto updateProp = [this, strFromVarBool]
        (const QString &name, const QDBusVariant &value) {
        auto it = setters_for_props_.find(name);
        if (it != setters_for_props_.end())
            it.value()(strFromVarBool(value.variant()));
    };
    bridge_.connect(bridge_.device_.get(), &Device::PropertyChanged
                    , updateProp);
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
