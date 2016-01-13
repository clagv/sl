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
        virtual SLVar       getVal (const std::string& key = std::string()) const = 0;
        virtual IDataRead*  getData(const std::string& key = std::string()) const = 0;

        virtual ~IDataRead(){}
    };

    class IDataWrite
    {
    public:
        // adds new existing key with value val
        virtual void newVal(const std::string& key, const SLVar& val) = 0;
        // overrides existing key with newVal
        virtual bool overwriteVal(const std::string& key, const SLVar& newVal, size_t keyIdx = 0) = 0;

        // pointer to new node keyed by key
        virtual IDataWrite* newData(const std::string& key) = 0;
        // pointer to existing keyIdx'th node (when there is more than one node with the same key) keyed by key
        virtual IDataWrite* existingData(const std::string& key, size_t keyIdx = 0) = 0;

        // remove entry (data or value) with specfied key and index keyIdx (if there are multiple entries with that key)
        virtual bool erase(const std::string& key, size_t keyIdx = 0) = 0;

        virtual ~IDataWrite(){}
    };
}