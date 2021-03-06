#ifndef _STATEFS_PROVIDERS_OFONO_DBUS_TYPEs_HPP_
#define _STATEFS_PROVIDERS_OFONO_DBUS_TYPEs_HPP_

#include <tuple>
#include <QList>
#include <statefs/qt/dbus.hpp>

typedef std::tuple<QDBusObjectPath, QVariantMap> PathProperties;
Q_DECLARE_METATYPE(PathProperties);

typedef QList<PathProperties> PathPropertiesArray;
Q_DECLARE_METATYPE(PathPropertiesArray);

static inline void registerDataTypes()
{
    qDBusRegisterMetaType<PathProperties>();
    qDBusRegisterMetaType<PathPropertiesArray>();
}

#endif // _STATEFS_PROVIDERS_OFONO_DBUS_TYPEs_HPP_
