#pragma once

#include "IData.h"
#include <vector>
#include <boost/shared_ptr.hpp>

namespace sl
{
    class IData : public IDataWrite, public IDataRead
    {
    public:
        // pointer to new (IdxNewEntry) or existing node (any other value for keyIdx if available)
        virtual IData* get_data(const std::string& key, size_t keyIdx = IdxNewEntry) = 0;
    };
    typedef boost::shared_ptr<IData> IDataPtr;

    IDataPtr createEmptyData();
}