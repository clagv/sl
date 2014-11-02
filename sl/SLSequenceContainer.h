#pragma once

#include "SLUtils.h"

namespace sl
{
   // Cont must have push_back, begin, end and swap
    template<typename Cont>
    class SLSequenceContainer : public ISerializable
    {
    public:
        typedef typename Cont::value_type value_type;

        SLSequenceContainer(const std::string& key, Cont& val, const std::string itemKey) 
            : m_key(key), m_itemKey(itemKey), m_val(&val)
        {
        }
        virtual void write(IDataWrite* data) const
        {
            if(!m_key.empty())
                data = data->getDataWrite(m_key);
            value_type vt;
            ISerializablePtr ser = makeSerializable(m_itemKey, vt);
            const Cont& val = *m_val;
            for(Cont::const_iterator it = val.begin(); it != val.end(); ++it)
            {
                vt = *it;
                ser->write(data);
            }
        }
        virtual void read(IDataRead* data)
        {
            if(!m_key.empty())
                data = data->getData(m_key);
            Cont val;
            value_type vt;
            ISerializablePtr ser = makeSerializable(m_itemKey, vt);
            for(data->begin(); !data->end(); data->next())
            {
                ser->read(data);
                val.push_back(vt);
            }
            val.swap(*m_val);
        }
    private:
        std::string         m_key;
        std::string         m_itemKey;
        Cont*               m_val;
    };
}