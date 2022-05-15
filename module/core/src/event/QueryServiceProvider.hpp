#ifndef BONGOMOUNTAIN_QUERYSERVICEPROVIDER_HPP
#define BONGOMOUNTAIN_QUERYSERVICEPROVIDER_HPP

#include "QueryServiceEvent.hpp"

class QueryServiceProvider
{
public:
    explicit QueryServiceProvider(QueryGenericServiceEvent &event)
        : event{ event }
    {}

    template<typename T, typename ProviderT>
    void provide(const ProviderT &provider)
    {
        if(event.getRequestType() != typeid(T))
            return;
        auto &query = reinterpret_cast<QueryServiceEvent<T> &>(event);
        auto  ptr   = provider(query.getInfo());
        if(ptr)
            query.addData(std::move(ptr));
    }

private:
    QueryGenericServiceEvent &event;
};

#endif// BONGOMOUNTAIN_QUERYSERVICEPROVIDER_HPP
