#include <statefs/qt/ns.hpp>
#include <QDebug>

namespace statefs { namespace qt {

Namespace::Namespace(char const *name
                     , std::unique_ptr<PropertiesSource> &&src)
    : statefs::Namespace(name)
    , src_(std::move(src))
{
}

void Namespace::setProperties(QVariantMap const &src)
{
    for (auto kv : setters_for_props_) {
        auto name = kv.first;
        auto psrc = src.find(name);
        if (psrc != src.end()) {
            auto set = kv.second;
            set(valueEncode(psrc.value()).toStdString());
        }
    }
}

void Namespace::setProperties(std::map<QString, QVariant> const &src)
{
    for (auto kv : setters_for_props_) {
        auto name = kv.first;
        auto psrc = src.find(name);
        if (psrc != src.end()) {
            auto set = kv.second;
            set(valueEncode(psrc->second).toStdString());
        }
    }
}

void Namespace::setProperties(DefaultProperties const &src)
{
    for (auto const &nv : src) {
        auto p = setters_for_props_.find(nv.first);
        if (p != setters_for_props_.end()) {
            auto set = p->second;
            set(std::string(nv.second));
        }
    }
}


void Namespace::updateProperty(const QString &name, const QVariant &value)
{
    auto it = setters_for_props_.find(name);
    if (it != setters_for_props_.end()) {
        auto &set = it->second;
        auto encoded = valueEncode(value);
        qDebug() << name << "=" << value << "->" << encoded;
        set(encoded.toStdString());
    }
}

void Namespace::addProperty(char const *name
                            , char const *def_val
                            , char const *src_name)
{
    using statefs::Discrete;
    auto d = Discrete(name, def_val);
    auto prop = d.create();
    *this << prop;
    setters_for_props_[src_name] = setter(prop);
}

void Namespace::addProperty(char const *name
                            , char const *def_val)
{
    addProperty(name, def_val, name);
}

void PropertiesSource::setProperties(QVariantMap const &src)
{
    target_->setProperties(src);
}

void PropertiesSource::setProperties(std::map<QString, QVariant> const &src)
{
    target_->setProperties(src);
}

void PropertiesSource::updateProperty
(const QString &name, const QVariant &value)
{
    target_->updateProperty(name, value);
}

}}
