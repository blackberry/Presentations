#include "Opt.hpp"

Opt::Opt(const Opt &opt)
    : m_text(opt.m_text)
    , m_mime(opt.m_mime)
    , m_uri(opt.m_uri)
    , m_data(opt.m_data)
    , m_action(opt.m_action)
    , m_description(opt.m_description)
{
}

Opt::Opt(const QString &text, const QString &description, const QString &mime, const QString &uri, const QString &data, const QString &action)
    : m_text(text)
    , m_mime(mime)
    , m_uri(uri)
    , m_data(data)
    , m_action(action)
    , m_description(description)
{
}

QString Opt::text() const
{
    return m_text;
}

QString Opt::mime() const
{
    return m_mime;
}

QString Opt::uri() const
{
    return m_uri;
}

QString Opt::data() const
{
    return m_data;
}

QString Opt::action() const
{
    return m_action;
}

QString Opt::description() const
{
    return m_description;
}
