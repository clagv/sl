#pragma once

#include <string>

namespace sl
{
    enum SLDataType {SLNone, SLBool, SLInt, SLLong, SLSize_t, SLDouble, SLString, SLData};

    class IDataRead
    {
    public:
        // start iterating at current level
        virtual void begin() = 0;
        // move to the next element
        virtual void next() = 0;
        // return true if there's no more data to read
        virtual bool end() const = 0;
        // current key
        virtual std::string key() const = 0;
        // query existence
        virtual bool hasKey(const std::string& key) const = 0;

        // inspect specific or current (empty key or key matchin current one) item
        virtual SLDataType  type     (const std::string& key = std::string()) const = 0;
        virtual bool        getBool  (const std::string& key = std::string()) const = 0;
        virtual int         getInt   (const std::string& key = std::string()) const = 0;
        virtual long        getLong  (const std::string& key = std::string()) const = 0;
        virtual size_t      getSize_t(const std::string& key = std::string()) const = 0;
        virtual double      getDouble(const std::string& key = std::string()) const = 0;
        virtual std::string getStr   (const std::string& key = std::string()) const = 0;
        virtual IDataRead*  getData  (const std::string& key = std::string()) const = 0;

        virtual ~IDataRead(){}
    };

    class IDataWrite
    {
    public:
        enum {IdxNewEntry = -1};

        // sets new (IdxNewEntry) or existing key (any other value, if available) with val
        virtual bool setVal(const std::string& key, bool               b, size_t keyIdx = IdxNewEntry) = 0;
        virtual bool setVal(const std::string& key, int                n, size_t keyIdx = IdxNewEntry) = 0;
        virtual bool setVal(const std::string& key, long               l, size_t keyIdx = IdxNewEntry) = 0;
        virtual bool setVal(const std::string& key, size_t             k, size_t keyIdx = IdxNewEntry) = 0;
        virtual bool setVal(const std::string& key, double             d, size_t keyIdx = IdxNewEntry) = 0;
        virtual bool setVal(const std::string& key, const std::string& s, size_t keyIdx = IdxNewEntry) = 0;
        bool setVal(const std::string& key, const char*        s, size_t keyIdx = IdxNewEntry) {return setVal(key, std::string(s), keyIdx);}
        // pointer to new (IdxNewEntry) or existing node (any other value for keyIdx if available)
        virtual IDataWrite* getDataWrite(const std::string& key, size_t keyIdx = IdxNewEntry) = 0;

        // remove entry (data or value) with specfied key and index keyIdx (if there are multiple entries with that key)
        virtual bool erase(const std::string& key, size_t keyIdx = 0) = 0;

        virtual ~IDataWrite(){}
    };

}