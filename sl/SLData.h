#pragma once

#include "IData.h"
#include <vector>
#include <boost/shared_ptr.hpp>

namespace sl
{
    class IData : public IDataWrite, public IDataRead
    {
    };
    typedef boost::shared_ptr<IData> IDataPtr;

    IDataPtr createEmptyData();
}