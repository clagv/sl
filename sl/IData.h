#pragma once

#include "SLVar.h"

namespace sl
{
    class IDataRead
    {
    public:
        // move to the first element at current level
        virtual void begin() = 0;
        // move to the next element
        virtual void next() = 0;
        // return true if there's no more data to read at this level
        virtual bool end() const = 0;
        // current key
        virtual std::string key() const = 0;
        // query existence
        virtual bool hasKey(const std::string& key) const = 0;

        // inspect specific or current (empty key or key matching current one) item
        virtual SLVar       getVar (const std::string& key = std::string()) const = 0;
        virtual IDataRead*  getData(const std::string& key = std::string()) const = 0;

        virtual ~IDataRead(){}
    };

    class IDataWrite
    {
    public:
        enum {IdxNewEntry = -1};

        // sets new (IdxNewEntry) or existing key (any other value, if available) with val
        virtual bool setVar(const std::string& key, const SLVar& v, size_t keyIdx = IdxNewEntry) = 0;
        // pointer to new (IdxNewEntry) or existing node (any other value for keyIdx if available)
        virtual IDataWrite* getDataWrite(const std::string& key, size_t keyIdx = IdxNewEntry) = 0;

        // remove entry (data or value) with specfied key and index keyIdx (if there are multiple entries with that key)
        virtual bool erase(const std::string& key, size_t keyIdx = 0) = 0;

        virtual ~IDataWrite(){}
    };
}