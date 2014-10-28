#pragma once

#include "SLUtils.h"
#include "SLPair.h"
#include <map>

namespace sl
{
    namespace detail
    {
        template<typename T>
        std::string pairKey() {return "item";}

        template<>
        std::string pairKey<std::string>() {return "";}
    }
    template<typename Map>
    class SLMap : public ISerializable
    {
    public:
        typedef typename Map::key_type key_type;
        typedef typename Map::mapped_type mapped_type;
        SLMap(const std::string& key, Map& m) : m_key(key), m_map(&m)
        {
        }
        virtual void write(IDataWrite* data) const
        {
            if(!m_key.empty())
                data = data->get_data(m_key);
            std::pair<key_type, mapped_type> tmp;
            ISerializablePtr ser = makeSerializable(detail::pairKey<key_type>(), tmp);
            for(Map::const_iterator it = m_map->begin(); it != m_map->end(); ++it)
            {
                tmp = *it;
                ser->write(data);
            }
        }
        virtual void read(IDataRead* data)
        {
            if(!m_key.empty())
                data = data->data(m_key);
            std::pair<key_type, mapped_type> tmp;
            ISerializablePtr ser = makeSerializable(detail::pairKey<key_type>(), tmp);
            Map m;
            for(data->begin(); !data->end(); data->next())
            {
                ser->read(data);
                m.insert(tmp);
            }
            m.swap(*m_map);
        }
    private:
        std::string m_key;
        Map*        m_map;
    };


    template<typename K, typename V>
    inline
    ISerializablePtr makeSerializable(const std::string& key, std::map<K, V>& val)
    {
        return ISerializablePtr(new SLMap<std::map<K, V>>(key, val));
    }
}