#ifndef _STATEFS_PRIVATE_CONNMAN_HPP_
#define _STATEFS_PRIVATE_CONNMAN_HPP_

#include "manager_interface.h"
#include "net_interface.h"
#include "sim_interface.h"
#include "stk_interface.h"

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
typedef OrgOfonoNetworkOperatorInterface Operator;
typedef OrgOfonoModemInterface Modem;
typedef OrgOfonoSimManagerInterface SimManager;
typedef OrgOfonoSimToolkitInterface SimToolkit;
using statefs::qt::ServiceWatch;

class MainNs;

class Bridge : public QObject, public statefs::qt::PropertiesSource
{
    Q_OBJECT;
public:
    Bridge(MainNs *, QDBusConnection &bus);

    virtual ~Bridge() {}

    virtual void init();

    typedef std::function<void(Bridge*, QVariant const&)> property_action_type;
    typedef std::map<QString, property_action_type> property_map_type;

    enum class Status {
        NoSim, Offline, Registered, Searching, Denied, Unknown, Roaming
            , EOE
            };

    void set_status(Status);
    Status map_status(QString const&);
    void set_network_name(QVariant const &);
    void set_operator_name(QVariant const &);
    void set_name_home();
    void set_name_roaming();

private:

    bool setup_modem(QString const &, QVariantMap const&);
    bool setup_operator(QString const &, QVariantMap const&);
    void setup_sim(QString const &);
    void setup_network(QString const &);
    void setup_stk(QString const &);
    void reset_sim();
    void reset_network();
    void reset_modem();
    void reset_stk();
    void process_features(QStringList const&);
    void enumerate_operators();

    QDBusConnection &bus_;
    std::unique_ptr<Manager> manager_;
    std::unique_ptr<Modem> modem_;
    std::unique_ptr<Network> network_;
    std::unique_ptr<Operator> operator_;
    std::unique_ptr<SimManager> sim_;
    std::unique_ptr<SimToolkit> stk_;
    ServiceWatch watch_;

    bool has_sim_;
    bool supports_stk_;
    Status status_;
    std::pair<QString, QString> network_name_;
    void (Bridge::*set_name_)();

    QString modem_path_;
    QString operator_path_;

    static const property_map_type net_property_actions_;
    static const property_map_type operator_property_actions_;
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
