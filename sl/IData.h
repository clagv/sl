#pragma once

#include <string>

namespace sl
{
//    enum DataType {DTNone, DTBool, DTInt, DTLong, DTSize_t, DTDouble, DTString, DTData};

    class IDataWrite
    {
    public:
        enum {IdxNewEntry = -1};
        // pointer to new (IdxNewEntry) or existing node (any other value for keyIdx if available)
        virtual IDataWrite* get_data(const std::string& key, size_t keyIdx = IdxNewEntry) = 0;
        // sets new (IdxNewEntry) or existing key (any other value, if available) with val
        virtual bool set_val(const std::string& key, const std::string& val, size_t keyIdx = IdxNewEntry) = 0;
        // remove entry (data or value) with specfied key and index keyIdx (if there are multiple entries with that key)
        virtual bool erase(const std::string& key, size_t keyIdx = 0) = 0;

        virtual ~IDataWrite(){}
    };

    class IDataRead
    {
    public:
        enum Type {None, Str, Data};
        // start iterating at current level
        virtual void begin() = 0;
        // move to the next element
        virtual void next() = 0;
        // return true if there's no more data to read
        virtual bool end() const = 0;
        // current key
        virtual std::string key() const = 0;
        // query existence
        virtual bool has_key(const std::string& key) const = 0;

        // inspect specific or current (empty key or key matchin current one) item
        virtual Type type(const std::string& key = std::string()) const = 0;
        virtual std::string str(const std::string& key = std::string()) const = 0;
        virtual IDataRead* data(const std::string& key = std::string()) const = 0;

        virtual ~IDataRead(){}
    };

}