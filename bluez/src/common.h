#ifndef _STATEFS_PROVIDER_BLUEZ_COMMON_H_
#define _STATEFS_PROVIDER_BLUEZ_COMMON_H_

#include <QList>
#include <QString>
#include <QtCore/QMetaType>
#include <QtDBus/QtDBus>

struct BluezService
{
    unsigned i;
    QString s;
};

Q_DECLARE_METATYPE(BluezService);

typedef QList<BluezService> ServiceMap;

QDBusArgument &operator <<(QDBusArgument &, BluezService const&);
QDBusArgument const& operator >>(QDBusArgument const&, BluezService&);

static inline void registerDataTypes()
{
    qDBusRegisterMetaType<BluezService>();
    qDBusRegisterMetaType<ServiceMap>();
}


#endif // _STATEFS_PROVIDER_BLUEZ_COMMON_H_
