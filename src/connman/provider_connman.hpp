#ifndef _STATEFS_PRIVATE_CONNMAN_HPP_
#define _STATEFS_PRIVATE_CONNMAN_HPP_

#include "manager_interface.h"
#include "service_interface.h"

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
typedef NetConnmanServiceInterface Service;
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

    enum Order {
        WiFi,
        Cellular,
        Other,

        OrderEnd
    };

    void init_manager();
    Status process_technology(QString const&, QVariantMap const&);
    Status process_services();
    void process_technologies();
    Status process_service(QString const&, QVariantMap const &);
    void reset_manager();
    void reset_properties();
    Order get_order(QString const&);

    QDBusConnection &bus_;
    std::unique_ptr<ServiceWatch> watch_;
    std::unique_ptr<Manager> manager_;
    std::unique_ptr<Service> service_;
    Order current_net_order_;
    QString current_technology_;
    QString current_service_;
};

class InternetNs : public statefs::qt::Namespace
{
public:
    InternetNs(QDBusConnection &bus);
private:
    friend class Bridge;
    void reset_properties();
    statefs::qt::DefaultProperties defaults_;
};

}}

#endif // _STATEFS_PRIVATE_CONNMAN_HPP_
