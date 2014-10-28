#pragma once

namespace sl
{
    class IDataWrite;
    class IDataRead;

    class IWritable
    {
    public:
        virtual void write(IDataWrite* data) const = 0;
        virtual ~IWritable(){}
    };

    class IReadable
    {
    public:
        virtual void read(IDataRead* data) = 0;
        virtual ~IReadable(){}
    };
    
    class ISerializable : public IReadable, public IWritable
    {
    public:
        virtual ~ISerializable(){}
    };
}