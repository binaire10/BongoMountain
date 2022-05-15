#ifndef BONGOMOUNTAIN_QUERYSERVICEEVENT_HPP
#define BONGOMOUNTAIN_QUERYSERVICEEVENT_HPP


#include <typeindex>
#include <memory>
#include <optional>
#include <list>
#include <any>

#include <core/config.hpp>
#include "Event.hpp"
#include <core/Platform.hpp>

class BM_CORE_DCL QueryGenericServiceEvent : public Event
{
public:
    EVENT_CLASS(Layer, QueryInterface)

    inline std::type_index getRequestType() noexcept { return m_requestType; }

protected:
    QueryGenericServiceEvent(std::type_index request_type)
        : m_requestType{ request_type }
    {}

private:
    std::type_index m_requestType;
};

template<typename InterT>
class QueryServiceEvent : public QueryGenericServiceEvent
{
public:
    inline QueryServiceEvent()
        : QueryGenericServiceEvent{ typeid(InterT) }
    {}

    inline std::list<std::unique_ptr<InterT>> get() { return std::move(m_data); }

    inline void addData(std::unique_ptr<InterT> &&value) { m_data.push_back(std::move(value)); }

    inline const std::optional<std::any> &getInfo() const { return m_info; }

    inline void setInfo(std::optional<std::any> &&value) { m_info = std::move(value); }

private:
    std::list<std::unique_ptr<InterT>> m_data;
    std::optional<std::any>            m_info;
};

template<typename T>
std::unique_ptr<T> make_service(core::ExtensionManager &platform)
{
    QueryServiceEvent<T> query;
    platform.dispatchEvent(query);
    return std::move(query.get().back());
}

template<typename T>
std::list<std::unique_ptr<T>> make_services(core::ExtensionManager &platform)
{
    QueryServiceEvent<T> query;
    platform.dispatchEvent(query);
    return query.get();
}

template<typename T>
std::unique_ptr<T> make_service(core::ExtensionManager &platform, std::any &&info)
{
    QueryServiceEvent<T> query;
    query.setInfo(std::move(info));
    platform.dispatchEvent(query);
    return std::move(query.get().back());
}

template<typename T>
std::list<std::unique_ptr<T>> make_services(core::ExtensionManager &platform, std::any &&info)
{
    QueryServiceEvent<T> query;
    query.setInfo(std::move(info));
    platform.dispatchEvent(query);
    return query.get();
}

#endif// BONGOMOUNTAIN_QUERYSERVICEEVENT_HPP
