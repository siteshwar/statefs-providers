#include <statefs/qt/ns.hpp>

namespace statefs { namespace qt {

Namespace::Namespace(char const *name
                     , std::unique_ptr<PropertiesSource> &&src)
    : statefs::Namespace(name)
    , src_(std::move(src))
{
}

void Namespace::setProperties(QVariantMap const &src)
{
    for (auto it = setters_for_props_.begin();
         it != setters_for_props_.end();
         ++it) {
        auto name = it.key();
        auto psrc = src.find(name);
        if (psrc != src.end()) {
            auto set = it.value();
            set(valueEncode(psrc.value()).toStdString());
        }
    }
}

void Namespace::updateProperty(const QString &name, const QVariant &value)
{
    auto it = setters_for_props_.find(name);
    if (it != setters_for_props_.end()) {
        auto &set = it.value();
        set(valueEncode(value).toStdString());
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

void PropertiesSource::updateProperty
(const QString &name, const QVariant &value)
{
    target_->updateProperty(name, value);
}

}}
