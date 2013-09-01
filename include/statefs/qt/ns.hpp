#ifndef _STATEFS_QT_NS_HPP_
#define _STATEFS_QT_NS_HPP_

#include <statefs/qt/util.hpp>
#include <statefs/property.hpp>

#include <map>
#include <QString>
#include <QVariant>

namespace statefs { namespace qt {

class Namespace;

typedef std::vector<std::pair<char const*, char const*> > DefaultProperties;

class PropertiesSource
{
public:
    PropertiesSource(Namespace *ns) : target_(ns) {}
    virtual ~PropertiesSource() {}
    virtual void init() =0;
    void setProperties(QVariantMap const &);
    void setProperties(std::map<QString, QVariant> const &);
    void updateProperty(const QString &, const QVariant &);
protected:
    Namespace *target_;
};

class Namespace : public statefs::Namespace
{
public:

    Namespace(char const *, std::unique_ptr<PropertiesSource> &&);

    virtual ~Namespace() {}
    virtual void release() { }

protected:
    void addProperty(char const *, char const *);
    void addProperty(char const *, char const *, char const *);
    void setProperties(DefaultProperties const &);

    std::unique_ptr<PropertiesSource> src_;

private:

    void setProperties(QVariantMap const &);
    void setProperties(std::map<QString, QVariant> const &);
    void updateProperty(const QString &, const QVariant &);

    friend class PropertiesSource;

    std::map<QString, setter_type> setters_for_props_;
};

}}

#endif // _STATEFS_QT_NS_HPP_
