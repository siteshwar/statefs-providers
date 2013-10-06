/*
 * StateFS Generic keyboard provider
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

#include "provider_keyboard_generic.hpp"

#include <cor/udev/util.hpp>
#include <linux/input.h>

#include <QVector>
#include <QStringList>
#include <QString>

namespace statefs { namespace keyboard {

using cor::udevpp::is_keyboard_available;
using statefs::qt::Namespace;
using statefs::qt::PropertiesSource;

Bridge::Bridge(KeyboardNs *ns)
    : PropertiesSource(ns)
{
}

void Bridge::init()
{
    auto v = is_keyboard_available();
    updateProperty("Present", v);
    updateProperty("Open", v);
}


KeyboardNs::KeyboardNs()
    : Namespace("maemo_InternalKeyboard", std::unique_ptr<PropertiesSource>
                (new Bridge(this)))
    , defaults_({{"Present", "0"}, {"Open", "0"}})
{
    for (auto v : defaults_)
        addProperty(v.first, v.second);
    src_->init();
}

void KeyboardNs::reset_properties()
{
    setProperties(defaults_);
}

class Provider;
static Provider *provider = nullptr;

class Provider : public statefs::AProvider
{
public:
    Provider(statefs_server *server)
        : AProvider("keyboard-generic", server)
    {
        auto ns = std::make_shared<KeyboardNs>();
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
    return statefs::keyboard::init_provider(server);
}
