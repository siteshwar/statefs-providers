#ifndef _STATEFS_PRIVATE_KEYBOARD_GENERIC_HPP_
#define _STATEFS_PRIVATE_KEYBOARD_GENERIC_HPP_

#include <statefs/provider.hpp>
#include <statefs/property.hpp>
#include <statefs/qt/ns.hpp>

#include <map>
#include <QDBusConnection>
#include <QString>
#include <QVariant>
#include <QObject>

namespace statefs { namespace keyboard {

class KeyboardNs;

class Bridge : public QObject, public statefs::qt::PropertiesSource
{
    Q_OBJECT;
public:
    Bridge(KeyboardNs *);
    virtual ~Bridge() {}
    virtual void init();

private:

};

class KeyboardNs : public statefs::qt::Namespace
{
public:
    KeyboardNs();

private:
    friend class Bridge;
    void reset_properties();

    statefs::qt::DefaultProperties defaults_;
};


}}

#endif // _STATEFS_PRIVATE_CONNMAN_HPP_
