#pragma once

#include "SLSequenceContainer.h"
#include <vector>

namespace sl
{
    template<typename T, typename A>
    struct SLVector : public SLSequenceContainer<std::vector<T, A>>
    {
        typedef std::vector<T, A> Cont;
        typedef SLSequenceContainer<Cont> SLCont;
        SLVector(const std::string& key, Cont& c, const std::string& itemKey) : SLCont(key, c, itemKey) {}
    };

    template<typename T, typename A>
    inline
    ISerializablePtr makeSerializable(const std::string& key, std::vector<T, A>& val, const std::string& itemKey = "item")
    {
        return ISerializablePtr(new SLVector<T, A>(key, val, itemKey));
    }
}