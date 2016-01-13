#pragma once

#include "SLPair.h"
#include "SLAssociativeContainer.h"
#include <map>


namespace sl
{
    template<typename K, typename V, typename C, typename A>
    struct SLMap : public SLAssociativeContainer<std::map<K, V, C, A>>
    {
        typedef std::map<K, V, C, A> Cont;
        typedef SLAssociativeContainer<Cont> SLCont;
        SLMap(const std::string& key, Cont& c) : SLCont(key, c) {}
    };

    template<typename K, typename V, typename C, typename A>
    struct SLMultiMap : public SLAssociativeContainer<std::multimap<K, V, C, A>>
    {
        typedef std::multimap<K, V, C, A> Cont;
        typedef SLAssociativeContainer<Cont> SLCont;
        SLMultiMap(const std::string& key, Cont& c) : SLCont(key, c) {}
    };

    template<typename K, typename V, typename C, typename A>
    inline
    ISerializablePtr makeSerializable(const std::string& key, std::map<K, V, C, A>& val)
    {
        return ISerializablePtr(new SLMap<K, V, C, A>(key, val));
    }

    template<typename K, typename V, typename C, typename A>
    inline
    ISerializablePtr makeSerializable(const std::string& key, std::multimap<K, V, C, A>& val)
    {
        return ISerializablePtr(new SLMultiMap<K, V, C, A>(key, val));
    }
}