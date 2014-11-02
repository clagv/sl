#pragma once

#include <utility>
#include <boost/shared_ptr.hpp>
#include "ISerializable.h"
#include "IData.h"

namespace sl
{
    class ISerializableObj;
    typedef boost::shared_ptr<ISerializable>    ISerializablePtr;
    typedef boost::shared_ptr<ISerializableObj> ISerializableObjPtr;

    class ISerializableObj : public ISerializable
    {
    public:
        virtual void add(const ISerializablePtr& ptr) = 0;
    };

    ISerializableObjPtr createSerializableObj(const std::string& key = std::string());

    inline
    void add(ISerializableObjPtr& to, const ISerializablePtr& ptr)
    {
        to->add(ptr);
    }

    template<typename T>
    inline
    void add(ISerializableObjPtr& to, const std::string& key, T& val)
    {
        add(to, makeSerializable(key, val));
    }
    ISerializablePtr makeSerializable(const std::string& key, bool&         val);
    ISerializablePtr makeSerializable(const std::string& key, int&          val);
    ISerializablePtr makeSerializable(const std::string& key, long&         val);
    ISerializablePtr makeSerializable(const std::string& key, size_t&       val);
    ISerializablePtr makeSerializable(const std::string& key, double&       val);
    ISerializablePtr makeSerializable(const std::string& key, std::string&  val);

}