#ifndef _STATEFS_PRIVATE_BLUEZ_HPP_
#define _STATEFS_PRIVATE_BLUEZ_HPP_

#include "manager_interface.h"
#include "device_interface.h"

#include <statefs/provider.hpp>
#include <statefs/property.hpp>
#include <statefs/qt/ns.hpp>
#include <statefs/qt/dbus.hpp>

#include <QObject>

namespace statefs { namespace upower {

typedef OrgFreedesktopUPowerInterface Manager;
typedef OrgFreedesktopUPowerDeviceInterface Device;

class PowerNs;

template <size_t Pos, typename T, typename ActionsT>
struct TupleOperations
{
    static void difference(T const &before, T const &current, ActionsT const &actions)
    {
        auto const &v1 = std::get<std::tuple_size<T>::value - Pos>(before);
        auto const &v2 = std::get<std::tuple_size<T>::value - Pos>(current);
        if (v2 != v1)
            std::get<std::tuple_size<T>::value - Pos>(actions)(v2);
        TupleOperations<Pos - 1, T, ActionsT>::difference(before, current, actions);
    }

    static void difference_update
    (T &before, T const &current, ActionsT const &actions)
    {
        auto &v1 = std::get<std::tuple_size<T>::value - Pos>(before);
        auto const &v2 = std::get<std::tuple_size<T>::value - Pos>(current);
        if (v2 != v1) {
            std::get<std::tuple_size<T>::value - Pos>(actions)(v2);
            v1 = v2;
        }
        TupleOperations<Pos - 1, T, ActionsT>::difference(before, current, actions);
    }
};

template <typename T, typename ActionsT>
struct TupleOperations<0, T, ActionsT>
{
    static void difference
    (T const &before, T const &current, ActionsT const &actions)
    {
    }
    static void difference_update
    (T &before, T const &current, ActionsT const &actions)
    {
    }
};

template <typename ActionsT, typename ...Args>
void process_difference
(std::tuple<Args...> const &before
 , std::tuple<Args...> const &current
 , ActionsT const &actions)
{
    typedef std::tuple<Args...> tuple_type;
    TupleOperations<std::tuple_size<tuple_type>::value,
                    tuple_type, ActionsT>::difference(before, current, actions);
}

template <typename ActionsT, typename ...Args>
void process_difference_update
(std::tuple<Args...> &before
 , std::tuple<Args...> const &current
 , ActionsT const &actions)
{
    typedef std::tuple<Args...> tuple_type;
    TupleOperations<
        std::tuple_size<tuple_type>::value,tuple_type, ActionsT>::difference_update
        (before, current, actions);
}

class Bridge : public QObject, public statefs::qt::PropertiesSource
{
    Q_OBJECT;
public:

    enum DeviceState {
        UnknownState = 0,
        Charging,
        Discharging,
        Empty,
        FullyCharged,
        PendingCharge,
        PendingDischarge
    };

    enum DeviceType {
        UnknownDevice,
        LinePower,
        Battery,
        Ups,
        Monitor,
        Mouse,
        Keyboard,
        Pda,
        Phone
    };

    Bridge(PowerNs *, QDBusConnection &bus);

    virtual ~Bridge() {}

    virtual void init();

private:

    void initManager();
    void updateAllProperties();
    bool findBattery();
    void watchUPower();

    QDBusConnection &bus_;
    QDBusObjectPath defaultAdapter_;
    std::unique_ptr<Manager> manager_;
    std::unique_ptr<Device> device_;
    statefs::qt::ServiceWatch watch_;

    typedef std::tuple<double, bool, bool, qlonglong,
                       qlonglong, DeviceState> Properties;
    Properties last_values_;
};

class PowerNs : public statefs::qt::Namespace
{
public:

    PowerNs(QDBusConnection &bus);
private:
    statefs::qt::DefaultProperties defaults_;
};

}}

#endif // _STATEFS_PRIVATE_BLUEZ_HPP_
