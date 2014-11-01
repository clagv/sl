#pragma once

#include "SLUtils.h"

namespace sl
{
    namespace detail
    {
        template<typename T>
        std::string pairKey() {return "item";}

        template<>
        std::string pairKey<std::string>() {return "";}

        template<typename T>
        struct SLNonConst
        {
            typedef typename T type;
        };

        template<typename T>
        struct SLNonConst<const T>
        {
            typedef typename T type;
        };

        template<typename T, typename U>
        struct SLNonConst<std::pair<T, U>>
        {
            typedef typename std::pair<typename SLNonConst<T>::type, typename SLNonConst<U>::type> type;
        };
    }
    template<typename Cont>
    class SLAssociativeContainer : public ISerializable
    {
    public:
        typedef typename Cont::key_type key_type;
        typedef typename detail::SLNonConst<typename Cont::value_type>::type type;
        SLAssociativeContainer(const std::string& key, Cont& c) : m_key(key), m_cont(&c)
        {
        }
        virtual void write(IDataWrite* data) const
        {
            if(!m_key.empty())
                data = data->getDataWrite(m_key);
            type tmp;
            ISerializablePtr ser = makeSerializable(detail::pairKey<key_type>(), tmp);
            for(Cont::const_iterator it = m_cont->begin(); it != m_cont->end(); ++it)
            {
                tmp = *it;
                ser->write(data);
            }
        }
        virtual void read(IDataRead* data)
        {
            if(!m_key.empty())
                data = data->data(m_key);
            type tmp;
            ISerializablePtr ser = makeSerializable(detail::pairKey<key_type>(), tmp);
            Cont c;
            for(data->begin(); !data->end(); data->next())
            {
                ser->read(data);
                c.insert(tmp);
            }
            c.swap(*m_cont);
        }
    private:
        std::string m_key;
        Cont*       m_cont;
    };

}