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

#include "provider_keyboard-generic.hpp"

#include <cor/udev.hpp>
#include <linux/input.h>

#include <QVector>
#include <QStringList>
#include <QString>

namespace statefs { namespace keyboard {

static bool is_keyboard_device(cor::udev::Device const &dev)
{
    QString key(dev.attr("capabilities/key"));
    QStringList caps_strs(key.split(' ', QString::SkipEmptyParts));
    if (caps_strs.isEmpty())
        return false;
    QVector<unsigned long> caps;
    foreach(QString const &s, caps_strs) {
        unsigned long v;
        bool is_ok = false;
        v = s.toULong(&is_ok, 16);
        if (!is_ok)
            return false;
        caps.push_back(v);
    }
    size_t count = 0;
    for (unsigned i = KEY_Q; i <= KEY_P; ++i) {
        int pos = caps.size() - (i / sizeof(unsigned long));
        if (pos < 0)
            break;
        size_t bit = i % sizeof(unsigned long);
        if ((caps[pos] >> bit) & 1)
            ++count;
    }
    return (count == KEY_P - KEY_Q);
}

static bool is_keyboard_available()
{
    using namespace cor::udev;
    Root udev;
    if (!udev)
        return false;

    Enumerate input(udev);
    if (!input)
        return false;

    input.subsystem_add("input");
    ListEntry devs(input);

    bool is_kbd_found = false;
    auto find_kbd = [&is_kbd_found, &udev](ListEntry const &e) -> bool {
        Device d(udev, e.path());
        is_kbd_found = is_keyboard_device(d);
        return !is_kbd_found;
    };
    devs.for_each(find_kbd);
    return is_kbd_found;
}

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
