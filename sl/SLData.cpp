#include "SLData.h"

namespace sl
{
    namespace
    {
        typedef std::pair<std::string, size_t> KeyIdx;

        int str_cmp(const std::string& l, const std::string& r)
        {
            return _stricmp(l.c_str(), r.c_str());
        }
        int str_eql(const std::string& l, const std::string& r)
        {
            return 0 == str_cmp(l, r);
        }

        int key_cmp(const KeyIdx& l, const KeyIdx& r)
        {
            return str_cmp(l.first, r.first);
        }
        bool key_less(const KeyIdx& l, const KeyIdx& r)
        {
            return key_cmp(l, r) < 0;
        }
        bool key_eql(const KeyIdx& l, const KeyIdx& r)
        {
            return key_cmp(l, r) == 0;
        }
        struct SLStore
        {
            explicit SLStore(const IDataPtr& data) : m_data(data)  {}
            explicit SLStore(const SLVar& var)     : m_var(var){}

            SLVar    m_var;
            IDataPtr m_data;
        };
    }
    class Data : public IData
    {
    public:
        Data() : m_pos(IdxInvalidPos) {}

        // start iterating at current level
        virtual void begin();
        // move to the next element
        virtual void next();
        // return true if there's no more data to read
        virtual bool end() const;
        // current key
        virtual std::string key() const;
        // query existence
        virtual bool hasKey(const std::string& key) const;

        // inspect specific or current (empty key or key matchin current one) item
        virtual SLVar       getVal   (const std::string& key = std::string()) const;
        virtual IDataRead*  getData  (const std::string& key = std::string()) const;

        // adds new existing key with value val
        virtual void newVal(const std::string& key, const SLVar& val);
        // overrides existing key with newVal
        virtual bool overwriteVal(const std::string& key, const SLVar& newVal, size_t keyIdx = 0);

        // pointer to new node keyed by key
        virtual IData* newData(const std::string& key);
        // pointer to existing keyIdx'th node (when there is more than one node with the same key) keyed by key
        virtual IData* existingData(const std::string& key, size_t keyIdx = 0);

        // remove entry (data or value) with specfied key and index keyIdx (if there are multiple entries with that key)
        virtual bool erase(const std::string& key, size_t keyIdx = 0);

    private:
        enum {IdxInvalidPos = -1};
        size_t newEntry(const std::string& key, const SLStore& dv);
        size_t curIdx(bool required) const;
        size_t idx(const std::string& key, size_t keyIdx, bool required) const;
        IData* getDataWrite(const std::string& key, size_t keyIdx);

        typedef std::pair<std::string, size_t> KeyIdx;
        typedef std::vector<SLStore> DataVarVec;
        typedef std::vector<KeyIdx> KeyIdxVec;
        typedef std::vector<std::string> StrVec;

        DataVarVec  m_varData;
        KeyIdxVec   m_keyIdx;
        StrVec      m_keys;
        size_t      m_pos;
    };

    void Data::begin()
    {
        m_pos = 0;
    }
    void Data::next()
    {
        ++m_pos;
    }
    bool Data::end() const
    {
        return !(m_pos < m_keys.size());
    }
    std::string Data::key() const
    {
        return m_keys[curIdx(true)];
    }
    bool Data::hasKey(const std::string& key) const
    {
        return IdxInvalidPos != idx(key, 0, false);
    }
    SLVar Data::getVal(const std::string& key) const
    {
        const SLStore& store = m_varData[idx(key, 0, true)];
        if(store.m_data)
            throw std::runtime_error("Key [" + key + "] is not a scalar");
        return store.m_var;
    }
    IDataRead* Data::getData(const std::string& key) const
    {
        const SLStore& store = m_varData[idx(key, 0, true)];
        if(!store.m_data)
            throw std::runtime_error("Key [" + key + "] is not a subtree");
        return store.m_data.get();
    }
    IData* Data::getDataWrite(const std::string& key, size_t keyIdx)
    {
        if(keyIdx < m_varData.size())
        {
            SLStore& store = m_varData[keyIdx];
            if(SLVTNone == store.m_var.type() && store.m_data)
                return store.m_data.get();
        }
        return 0;
    }
    bool Data::erase(const std::string& key, size_t keyIdx)
    {
        if(keyIdx < m_keyIdx.size())
        {
            KeyIdx ki(key, 0);
            size_t pos = (std::lower_bound(m_keyIdx.begin(), m_keyIdx.end(), ki, key_less) - m_keyIdx.begin()) + keyIdx;
            if(pos < m_keyIdx.size() && key_eql(ki, m_keyIdx[pos]))
            {
                // users shouldn't really erase keys while reading data but ...
                if(m_pos < m_keyIdx.size() && m_pos > pos)
                    --m_pos;
                m_keyIdx.erase(m_keyIdx.begin() + pos);
                m_keys.erase(m_keys.begin() + pos);
                m_varData.erase(m_varData.begin() + pos);
                return true;
            }
        }
        return false;
    }
    size_t Data::newEntry(const std::string& key, const SLStore& dv)
    {
        KeyIdx ki(key, m_keyIdx.size());
        KeyIdxVec::iterator it = std::upper_bound(m_keyIdx.begin(), m_keyIdx.end(), ki, key_less);
        m_keys.push_back(key);
        m_varData.push_back(dv);
        m_keyIdx.insert(it, ki);
        return ki.second;
    }
    size_t Data::curIdx(bool required) const
    {
        // special case for single value - no need to call begin()
        if(1 == m_keys.size() && IdxInvalidPos == m_pos)
            return 1;
        if(!end())
            return m_pos;
        if(required)
            throw std::exception("Invalid sl::Data index");
        return IdxInvalidPos;
    }
    size_t Data::idx(const std::string& key, size_t keyCount, bool required) const
    {
        if(key.empty())
            return curIdx(required);
        if(0 == keyCount && !end() && str_eql(key, m_keys[m_pos]))
            return m_pos;
        if(keyCount < m_keyIdx.size())
        {
            KeyIdx ki(key, 0);
            KeyIdxVec::const_iterator it = std::lower_bound(m_keyIdx.begin(), m_keyIdx.end(), ki, key_less);
            size_t keyIdxPos = std::distance(m_keyIdx.begin(), it) + keyCount;
            if(keyIdxPos < m_keyIdx.size() && key_eql(ki, m_keyIdx[keyIdxPos]))
                return m_keyIdx[keyIdxPos].second;
        }
        if(required)
            throw std::exception("sl::Data::idx - key not found");
        return IdxInvalidPos;
    }
    void Data::newVal(const std::string& key, const SLVar& val)
    {
        newEntry(key, SLStore(val));
    }
    bool Data::overwriteVal(const std::string& key, const SLVar& newVal, size_t keyIdx)
    {
        size_t pos =  idx(key, keyIdx, false);
        if(pos < m_varData.size())
        {
            m_varData[pos] = SLStore(newVal);
            return true;
        }
        return false;
    }
    IData* Data::newData(const std::string& key)
    {
        size_t pos = newEntry(key, SLStore(createEmptyData()));
        return getDataWrite(key, pos);
    }
    IData* Data::existingData(const std::string& key, size_t keyIdx)
    {
        size_t pos = idx(key, keyIdx, false);
        return getDataWrite(key, pos);
    }


    IDataPtr createEmptyData()
    {
        return IDataPtr(new Data);
    }

}