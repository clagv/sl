#pragma once

#include <string>
#include <stdexcept>

namespace sl
{
    // SLVTStr must be 0
    enum SLVarType {SLVTStr, SLVTDbl, SLVTSzt, SLVTLng, SLVTInt, SLVTBool, SLVTNone};
    class SLVarTypeErr : public std::runtime_error
    {
    public:
        SLVarTypeErr(const std::string& str);
    };

    class SLVar
    {
    public:
        SLVar() : m_type(SLVTNone) {}
        explicit SLVar(const std::string& s)  { set_str(s.size(), s.c_str()); }
        explicit SLVar(const char* s)         { set_str(strlen(s), s); }
        explicit SLVar(double d)              : m_type(SLVTDbl)  { m_u.m_dbl  = d; }
        explicit SLVar(size_t k)              : m_type(SLVTSzt)  { m_u.m_szt  = k; }
        explicit SLVar(long l)                : m_type(SLVTLng)  { m_u.m_lng  = l; }
        explicit SLVar(int i)                 : m_type(SLVTInt)  { m_u.m_int  = i; }
        explicit SLVar(bool b)                : m_type(SLVTBool) { m_u.m_bool = b; }
        SLVar(const SLVar& v);
        ~SLVar();
        SLVar& operator=(const SLVar& v);

        void swap(SLVar& v);

        SLVarType type() const;

        const char*     get_chr() const;
        std::string     get_str()  const { return std::string(get_chr()); }
        double          get_dbl()  const { assertType(SLVTDbl);  return m_u.m_dbl;  }
        size_t          get_szt()  const { assertType(SLVTSzt);  return m_u.m_szt;  }
        long            get_lng()  const { assertType(SLVTLng);  return m_u.m_lng;  }
        int             get_int()  const { assertType(SLVTInt);  return m_u.m_int;  }
        bool            get_bool() const { assertType(SLVTBool); return m_u.m_bool; }

    private:
        enum {maxBuf = sizeof(double), VPtr = INT_MAX};

        void assertType(SLVarType t) const;
        void set_str(size_t len, const char* src);

        int m_type;
        union VarUni    // give it a type so that std::swap can work
        {
            char    m_buf[maxBuf];
            char*   m_ptr;
            double  m_dbl;
            size_t  m_szt;
            long    m_lng;
            int     m_int;
            bool    m_bool;
        }m_u;
    };
} // namespace