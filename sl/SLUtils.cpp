#include "SLUtils.h"
#include <vector>

namespace sl
{
 //   namespace
 //   {
        typedef std::vector<ISerializablePtr> Serializables;
        typedef std::vector<std::string> StrVec;

        struct StrSerializable : public ISerializable
        {
            StrSerializable(const std::string& key, std::string& val) : m_key(key), m_val(&val) {}
            virtual void write(IDataWrite* data) const
            {
                data->set_val(m_key, *m_val);
            }
            virtual void read(IDataRead* data)
            {
                *m_val = data->str(m_key);
            }
        private:
            std::string* m_val;
            std::string  m_key;
        };

        struct SerializableObj : public ISerializableObj
        {
            SerializableObj(const std::string& objKey) : m_objKey(objKey) {}
            virtual void write(IDataWrite* data) const
            {
                IDataWrite* objData = dataForKey(m_objKey, data);
                for(size_t k = 0; k < m_serializables.size(); ++k)
                    m_serializables[k]->write(objData);
            }
            virtual void read(IDataRead* data)
            {
                IDataRead* objData = dataForKey(m_objKey, data);
                for(size_t k = 0; k < m_serializables.size(); ++k)
                    m_serializables[k]->read(objData);
            }
            virtual void add(const ISerializablePtr& ptr)
            {
                m_serializables.push_back(ptr);
            }
        private:
            IDataWrite* dataForKey(const std::string& key, IDataWrite* data) const
            {
                return key.empty() ? data : data->get_data(key);
            }
            IDataRead* dataForKey(const std::string& key, IDataRead* data) const
            {
                return key.empty() ? data : data->data(key);
            }
            std::string     m_objKey;
            Serializables   m_serializables;
        };
//    }

    ISerializableObjPtr createSerializableObj(const std::string& key)
    {
        return ISerializableObjPtr(new SerializableObj(key));
    }
    ISerializablePtr makeSerializable(const std::string& key, std::string& val)
    {
        return ISerializablePtr(new StrSerializable(key, val));
    }
}