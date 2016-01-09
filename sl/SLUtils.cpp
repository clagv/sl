#include "SLUtils.h"
#include <vector>

namespace sl
{
    namespace
    {
        typedef std::vector<ISerializablePtr> Serializables;
        typedef std::vector<std::string> StrVec;

        void getDataType(IDataRead* data, const std::string& key, bool&         b){b = data->getVar(key).get_bool();}
        void getDataType(IDataRead* data, const std::string& key, long&         l){l = data->getVar(key).get_lng();}
        void getDataType(IDataRead* data, const std::string& key, int&          n){n = data->getVar(key).get_int();}
        void getDataType(IDataRead* data, const std::string& key, size_t&       k){k = data->getVar(key).get_szt();}
        void getDataType(IDataRead* data, const std::string& key, double&       x){x = data->getVar(key).get_dbl();}
        void getDataType(IDataRead* data, const std::string& key, std::string&  s){s = data->getVar(key).get_chr();}

        template<typename T>
        struct SLVarSerializable : public ISerializable
        {
            SLVarSerializable(const std::string& key, T& val) : m_key(key), m_val(&val) {}
            virtual void write(IDataWrite* data) const
            {
                data->setVar(m_key, SLVar(*m_val));
            }
            virtual void read(IDataRead* data)
            {
                getDataType(data, m_key, *m_val);
            }
        protected:
            std::string m_key;
            T*          m_val;
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
                return key.empty() ? data : data->getDataWrite(key);
            }
            IDataRead* dataForKey(const std::string& key, IDataRead* data) const
            {
                return key.empty() ? data : data->getData(key);
            }
            std::string     m_objKey;
            Serializables   m_serializables;
        };
    }

    ISerializableObjPtr createSerializableObj(const std::string& key)
    {
        return ISerializableObjPtr(new SerializableObj(key));
    }
    ISerializablePtr makeSerializable(const std::string& key, bool&         val)
    {
        return ISerializablePtr(new SLVarSerializable<bool>(key, val));
    }
    ISerializablePtr makeSerializable(const std::string& key, int&          val)
    {
        return ISerializablePtr(new SLVarSerializable<int>(key, val));
    }
    ISerializablePtr makeSerializable(const std::string& key, long&         val)
    {
        return ISerializablePtr(new SLVarSerializable<long>(key, val));
    }
    ISerializablePtr makeSerializable(const std::string& key, size_t&       val)
    {
        return ISerializablePtr(new SLVarSerializable<size_t>(key, val));
    }
    ISerializablePtr makeSerializable(const std::string& key, double&       val)
    {
        return ISerializablePtr(new SLVarSerializable<double>(key, val));
    }
    ISerializablePtr makeSerializable(const std::string& key, std::string&  val)
    {
        return ISerializablePtr(new SLVarSerializable<std::string>(key, val));
    }
}