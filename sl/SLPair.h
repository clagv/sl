#pragma once

#include <utility>
#include "SLUtils.h"

namespace sl
{
    namespace detail
    {
        template<typename T, typename U>
        struct SLPairMaker
        {
            static ISerializablePtr make(const std::string& key, std::pair<T, U>& val)
            {
                std::string curKey, subKey;
                curKey = splitKey(key, subKey);
                ISerializableObjPtr obj = createSerializableObj(curKey);
                curKey = subKey.empty() ? "key" : splitKey(subKey, subKey);
                add(obj, makeSerializable(curKey, val.first));
                curKey = subKey.empty() ? "val" : splitKey(subKey, subKey);
                add(obj, makeSerializable(curKey, val.second));
                return obj;
            }
        };



        template<typename U>
        struct SLPairMaker<std::string, U>
        {
            struct SLStrPair : public ISerializable
            {
                SLStrPair(std::string& key, U& val) : m_key(&key), m_val(&val) {}
                virtual void write(IDataWrite* data) const
                {
                    ISerializablePtr ser = makeSerializable(*m_key, *m_val);
                    ser->write(data);
                }
                virtual void read(IDataRead* data)
                {
                    if(data->end())
                        data->begin();
                    *m_key = data->key();
                    ISerializablePtr ser = makeSerializable(*m_key, *m_val);
                    ser->read(data);
                }
            private:
                std::string*    m_key;
                U*              m_val;
            };
            static ISerializablePtr make(const std::string& key, std::pair<std::string, U>& val)
            {
                ISerializableObjPtr obj = createSerializableObj(key);
                add(obj, ISerializablePtr(new SLStrPair(val.first, val.second)));
                return obj;
            }
        };

    }
    template<typename T, typename U>
    ISerializablePtr makeSerializable(const std::string& key, std::pair<T, U>& val)
    {
        return detail::SLPairMaker<T,U>::make(key, val);
    }
}