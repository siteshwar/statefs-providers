#ifndef _STATEFS_PRIVATE_CONNMAN_HPP_
#define _STATEFS_PRIVATE_CONNMAN_HPP_

#include "manager_interface.h"

#include <statefs/provider.hpp>
#include <statefs/property.hpp>
#include <statefs/qt/ns.hpp>
#include <statefs/qt/dbus.hpp>

#include <map>
#include <QDBusConnection>
#include <QString>
#include <QVariant>
#include <QObject>

namespace statefs { namespace connman {

typedef NetConnmanManagerInterface Manager;
using statefs::qt::ServiceWatch;

class InternetNs;

class Bridge : public QObject, public statefs::qt::PropertiesSource
{
    Q_OBJECT;
public:
    Bridge(InternetNs *, QDBusConnection &bus);

    virtual ~Bridge() {}

    virtual void init();

private:

    enum Status {
        ExactMatch,
        Match,
        Ignore
    };

    void initManager();
    Status processTechnology(QVariantMap const &);
    Status processServices();
    void processTechnologies();
    Status processService(QVariantMap const &);
    void resetManager();

    QDBusConnection &bus_;
    std::unique_ptr<ServiceWatch> watch_;
    std::unique_ptr<Manager> manager_;
    const std::map<QString, QVariant> defaults_;
};

class InternetNs : public statefs::qt::Namespace
{
public:
    InternetNs(QDBusConnection &bus);
};

}}

#endif // _STATEFS_PRIVATE_CONNMAN_HPP_
