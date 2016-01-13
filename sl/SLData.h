#pragma once

#include "IData.h"
#include <vector>
#include <boost/shared_ptr.hpp>

namespace sl
{
    class IData : public IDataWrite, public IDataRead
    {
    public:
        // pointer to new node keyed by key
        virtual IData* newData(const std::string& key) = 0;
        // pointer to existing keyIdx'th node (when there is more than one node with the same key) keyed by key
        virtual IData* existingData(const std::string& key, size_t keyIdx = 0) = 0;
    };
    typedef boost::shared_ptr<IData> IDataPtr;

    IDataPtr createEmptyData();
}