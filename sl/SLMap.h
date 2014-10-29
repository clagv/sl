#pragma once

#include "SLPair.h"
#include "SLAssociativeContainer.h"
#include <map>


namespace sl
{
    template<typename K, typename V>
    struct SLMap : public SLAssociativeContainer<std::map<K, V>>
    {
        typedef std::map<K, V> Cont;
        typedef SLAssociativeContainer<Cont> SLCont;
        SLMap(const std::string& key, Cont& c) : SLCont(key, c) {}
    };

    template<typename K, typename V>
    inline
    ISerializablePtr makeSerializable(const std::string& key, std::map<K, V>& val)
    {
        return ISerializablePtr(new SLMap<K, V>(key, val));
    }
}