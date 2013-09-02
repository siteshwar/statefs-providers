#ifndef _STATEFS_PRIVATE_CONNMAN_HPP_
#define _STATEFS_PRIVATE_CONNMAN_HPP_

#include "manager_interface.h"
#include "net_interface.h"
#include "sim_interface.h"

#include <statefs/provider.hpp>
#include <statefs/property.hpp>
#include <statefs/qt/ns.hpp>
#include <statefs/qt/dbus.hpp>

#include <map>
#include <QDBusConnection>
#include <QString>
#include <QVariant>
#include <QObject>

namespace statefs { namespace ofono {

typedef OrgOfonoManagerInterface Manager;
typedef OrgOfonoNetworkRegistrationInterface Network;
typedef OrgOfonoModemInterface Modem;
typedef OrgOfonoSimManagerInterface SimManager;
using statefs::qt::ServiceWatch;

class MainNs;

class Bridge : public QObject, public statefs::qt::PropertiesSource
{
    Q_OBJECT;
public:
    Bridge(MainNs *, QDBusConnection &bus);

    virtual ~Bridge() {}

    virtual void init();

private:

    bool setup_modem(QString const &, QVariantMap const&);
    void setup_sim(QString const &);
    void setup_network(QString const &);
    void reset_sim();
    void reset_network();
    void reset_modem();
    void process_features(QStringList const&);

    QDBusConnection &bus_;
    ServiceWatch watch_;
    bool has_sim_;
    std::unique_ptr<Manager> manager_;
    std::unique_ptr<Modem> modem_;
    std::unique_ptr<Network> network_;
    std::unique_ptr<SimManager> sim_;
    QString modem_path_;
    std::map<QString, std::pair<char const *, char const *> > tech_map_;
    std::map<QString, QString> status_map_;
    std::map<QString, std::function<void(QVariant const&)> >
    net_property_actions_;
    std::map<QString, QString> sim_props_map_;
};

class MainNs : public statefs::qt::Namespace
{
public:
    MainNs(QDBusConnection &bus);

private:
    friend class Bridge;
    enum Properties {
        Default,
        NoSimDefault
    };
    void resetProperties(Properties);

    statefs::qt::DefaultProperties defaults_;
};

}}

#endif // _STATEFS_PRIVATE_CONNMAN_HPP_
