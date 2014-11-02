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
        class SLVariant
        {
        public:
            explicit SLVariant(bool b)                    : m_type(SLBool)                {m_u.b = b;}
            explicit SLVariant(int n)                     : m_type(SLInt)                 {m_u.n = n;}
            explicit SLVariant(long l)                    : m_type(SLLong)                {m_u.l = l;}
            explicit SLVariant(size_t k)                  : m_type(SLSize_t)              {m_u.k = k;}
            explicit SLVariant(double d)                  : m_type(SLDouble)              {m_u.d = d;}
            explicit SLVariant(const std::string& str)    : m_type(SLString), m_str(str)  {}
            explicit SLVariant(const IDataPtr& data)      : m_type(SLData), m_data(data)  {}
            SLDataType type() const {return m_type;}
            bool getBool()                 const {assertType(SLBool);      return m_u.b; }
            int  getInt()                  const {assertType(SLInt);       return m_u.n; }
            long getLong()                 const {assertType(SLLong);      return m_u.l; }
            size_t getSize_t()             const {assertType(SLSize_t);    return m_u.k; }
            double getDouble()             const {assertType(SLDouble);    return m_u.d; }
            const std::string& getStr()    const {assertType(SLString);    return m_str; }
            const IDataPtr& getData()      const {assertType(SLData);      return m_data; }
        private:
            void assertType(SLDataType t) const
            {
                if(m_type != t)
                    throw std::exception("SLVariant: wrong type requested");
            }
            SLDataType      m_type;
            std::string     m_str;
            IDataPtr        m_data;
            union
            {
                bool    b;
                int     n;
                long    l;
                size_t  k;
                double  d;
            }m_u;
        };
    }
    class Data : public IData
    {
    public:
        Data() : m_pos(IdxNewEntry) {}

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
        virtual SLDataType  type     (const std::string& key = std::string()) const;
        virtual bool        getBool  (const std::string& key = std::string()) const;
        virtual int         getInt   (const std::string& key = std::string()) const;
        virtual long        getLong  (const std::string& key = std::string()) const;
        virtual size_t      getSize_t(const std::string& key = std::string()) const;
        virtual double      getDouble(const std::string& key = std::string()) const;
        virtual std::string getStr   (const std::string& key = std::string()) const;
        virtual IDataRead*  getData  (const std::string& key = std::string()) const;

        // sets new (IdxNewEntry) or existing key (any other value, if available) with val
        virtual bool setVal(const std::string& key, bool               b, size_t keyIdx = IdxNewEntry);
        virtual bool setVal(const std::string& key, int                n, size_t keyIdx = IdxNewEntry);
        virtual bool setVal(const std::string& key, long               l, size_t keyIdx = IdxNewEntry);
        virtual bool setVal(const std::string& key, size_t             k, size_t keyIdx = IdxNewEntry);
        virtual bool setVal(const std::string& key, double             d, size_t keyIdx = IdxNewEntry);
        virtual bool setVal(const std::string& key, const std::string& s, size_t keyIdx = IdxNewEntry);
        // pointer to new (IdxNewEntry) or existing node (any other value for keyIdx if available)
        virtual IData* getDataWrite(const std::string& key, size_t keyIdx = IdxNewEntry);

        // remove entry (data or value) with specfied key and index keyIdx (if there are multiple entries with that key)
        virtual bool erase(const std::string& key, size_t keyIdx = 0);

    private:
        size_t newEntry(const std::string& key, const SLVariant& dv);
        size_t curIdx(bool required) const;
        size_t idx(const std::string& key, size_t keyIdx, bool required) const;
        bool setVal(const std::string& key, const SLVariant& dv, size_t keyIdx);
        typedef std::pair<std::string, size_t> KeyIdx;
        typedef std::vector<SLVariant> DataVarVec;
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
        return IdxNewEntry != idx(key, 0, false);
    }
    SLDataType Data::type(const std::string& key) const
    {
        return m_varData[idx(key, 0, true)].type();
    }
    bool Data::getBool(const std::string& key) const
    {
        return m_varData[idx(key, 0, true)].getBool();
    }
    int Data::getInt(const std::string& key) const
    {
        return m_varData[idx(key, 0, true)].getInt();
    }
    long Data::getLong(const std::string& key) const
    {
        return m_varData[idx(key, 0, true)].getLong();
    }
    size_t Data::getSize_t(const std::string& key) const
    {
        return m_varData[idx(key, 0, true)].getSize_t();
    }
    double Data::getDouble(const std::string& key) const
    {
        return m_varData[idx(key, 0, true)].getDouble();
    }
    std::string Data::getStr(const std::string& key) const
    {
        return m_varData[idx(key, 0, true)].getStr();
    }
    IDataRead* Data::getData(const std::string& key) const
    {
        return m_varData[idx(key, 0, true)].getData().get();
    }
    bool Data::setVal(const std::string& key, bool b, size_t keyIdx)
    {
        return setVal(key, SLVariant(b), keyIdx);
    }
    bool Data::setVal(const std::string& key, int n, size_t keyIdx)
    {
        return setVal(key, SLVariant(n), keyIdx);
    }
    bool Data::setVal(const std::string& key, long l, size_t keyIdx)
    {
        return setVal(key, SLVariant(l), keyIdx);
    }
    bool Data::setVal(const std::string& key, size_t k, size_t keyIdx)
    {
        return setVal(key, SLVariant(k), keyIdx);
    }
    bool Data::setVal(const std::string& key, double d, size_t keyIdx)
    {
        return setVal(key, SLVariant(d), keyIdx);
    }
    bool Data::setVal(const std::string& key, const std::string& s, size_t keyIdx)
    {
        return setVal(key, SLVariant(s), keyIdx);
    }
    IData* Data::getDataWrite(const std::string& key, size_t keyIdx)
    {
        size_t pos = (IdxNewEntry == keyIdx) ? newEntry(key, SLVariant(createEmptyData())) : idx(key, keyIdx, false);
        if(pos < m_varData.size())
            return m_varData[pos].getData().get();
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
                    --pos;
                m_keyIdx.erase(m_keyIdx.begin() + pos);
                m_keys.erase(m_keys.begin() + pos);
                m_varData.erase(m_varData.begin() + pos);
                return true;
            }
        }
        return false;
    }
    size_t Data::newEntry(const std::string& key, const SLVariant& dv)
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
        if(1 == m_keys.size() && IdxNewEntry == m_pos)
            return 1;
        if(!end())
            return m_pos;
        if(required)
            throw std::exception("Invalid sl::Data index");
        return IdxNewEntry;
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
        return IdxNewEntry;
    }
    bool Data::setVal(const std::string& key, const SLVariant& dv, size_t keyIdx)
    {
        if(IdxNewEntry == keyIdx) 
        {
            newEntry(key, dv);
            return true;
        }
        size_t pos =  idx(key, keyIdx, false);
        if(pos < m_varData.size())
        {
            m_varData[pos] = dv;
            return true;
        }
        return false;
    }


    IDataPtr createEmptyData()
    {
        return IDataPtr(new Data);
    }

}