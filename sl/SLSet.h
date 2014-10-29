#pragma once

#include "SLAssociativeContainer.h"
#include <set>


namespace sl
{
    template<typename T>
    struct SLSet : public SLAssociativeContainer<std::set<T>>
    {
        typedef std::set<T> Cont;
        typedef SLAssociativeContainer<Cont> SLCont;
        SLSet(const std::string& key, Cont& c) : SLCont(key, c) {}
    };

    template<typename T>
    inline
    ISerializablePtr makeSerializable(const std::string& key, std::set<T>& val)
    {
        return ISerializablePtr(new SLSet<T>(key, val));
    }
}