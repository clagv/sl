#pragma once

#include "SLAssociativeContainer.h"
#include <set>


namespace sl
{
    template<typename T, typename C, typename A>
    struct SLSet : public SLAssociativeContainer<std::set<T, C, A>>
    {
        typedef std::set<T, C, A> Cont;
        typedef SLAssociativeContainer<Cont> SLCont;
        SLSet(const std::string& key, Cont& c) : SLCont(key, c) {}
    };

    template<typename T, typename C, typename A>
    inline
    ISerializablePtr makeSerializable(const std::string& key, std::set<T, C, A>& val)
    {
        return ISerializablePtr(new SLSet<T, C, A>(key, val));
    }
}