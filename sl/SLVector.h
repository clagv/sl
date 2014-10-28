#pragma once

#include "SLUtils.h"
#include <vector>

namespace sl
{
    // Vec must have push_back, operator[], size and swap
    template<typename Vec>
    class SLVector : public ISerializable
    {
    public:
        typedef typename Vec::value_type value_type;
        SLVector(const std::string& key, Vec& val) 
            : m_val(&val), m_t()
        {
            std::string keyItem;
            m_key = splitKey(key, keyItem);
            m_ser = makeSerializable(keyItem.empty() ? "item" : keyItem, m_t);
        }
        virtual void write(IDataWrite* data) const
        {
            if(!m_key.empty())
                data = data->get_data(m_key);
            Vec& val = *m_val;
            for(size_t k = 0; k < val.size(); ++k)
            {
                m_t = val[k];
                m_ser->write(data);
            }
        }
        virtual void read(IDataRead* data)
        {
            if(!m_key.empty())
                data = data->data(m_key);
            Vec val;
            for(data->begin(); !data->end(); data->next())
            {
                m_ser->read(data);
                val.push_back(m_t);
            }
            val.swap(*m_val);
        }
    private:
        std::string         m_key;
        Vec*                m_val;
        mutable value_type  m_t;
        ISerializablePtr    m_ser;
    };

    template<typename T>
    inline
    ISerializablePtr makeSerializable(const std::string& key, std::vector<T>& val)
    {
        return ISerializablePtr(new SLVector<std::vector<T>>(key, val));
    }
}