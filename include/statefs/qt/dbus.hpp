#ifndef _STATEFS_QT_DBUS_HPP_
#define _STATEFS_QT_DBUS_HPP_

#include <QDBusPendingReply>
#include <stdexcept>
#include <tuple>

namespace statefs { namespace qt {

template <typename T>
QDBusPendingReply<T> sync(QDBusPendingReply<T> &&reply)
{
    QDBusPendingCallWatcher watcher(reply);
    watcher.waitForFinished();
    if (!watcher.isFinished())
        throw std::logic_error("D-Bus request is not executed");
    return reply;
}

template <size_t Pos, typename T>
struct TupleDBus
{
    static void output(QDBusArgument &argument, T &src)
    {
        argument << std::get<std::tuple_size<T>::value - Pos>(src);
        TupleDBus<Pos - 1, T>::output(argument, src);
    }
};


template <typename T>
struct TupleDBus<0, T>
{
    static void output(QDBusArgument &argument, T &src)
    {
        argument << std::get<std::tuple_size<T>::value>(src);
    }
};

template <typename ... Args>
QDBusArgument & operator << (QDBusArgument &argument, std::tuple<Args...> &src)
{
    typedef std::tuple<Args...> tuple_type;
    argument.beginStructure();
    TupleDBus<std::tuple_size<tuple_type>::value
              , tuple_type>::output(argument, src);
    argument.endStructure();
    return argument;
}

}}

#endif // _STATEFS_QT_DBUS_HPP_
