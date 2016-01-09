#include "SLVar.h"

namespace sl
{
    SLVarTypeErr::SLVarTypeErr(const std::string& str) : std::runtime_error(str) {}
    void assertType(SLVarType req, SLVarType actual)
    {
        if(req != actual)
            throw SLVarTypeErr("Incorrect type required");
    }
    SLVar::SLVar(const SLVar& v) : m_type(v.m_type)
    {
        if(VPtr == m_type)
        {
            size_t bufSz = strlen(v.m_u.m_ptr) + 1;
            m_u.m_ptr = new char[bufSz];
            strcpy_s(m_u.m_ptr, bufSz, v.m_u.m_ptr);
        }
        else
        {
            m_u = v.m_u;
        }
    }
    SLVar::~SLVar() {
        if(VPtr == m_type)
            delete [] m_u.m_ptr;
    }
    SLVar& SLVar::operator=(const SLVar& v)
    {
        SLVar t(v);
        swap(t);
        return *this;
    }

    void SLVar::swap(SLVar& v)
    {
        std::swap(m_type,   v.m_type);
        std::swap(m_u,      v.m_u);
    }

    SLVarType SLVar::type() const { return static_cast<SLVarType>(m_type%VPtr); }

    const char*     SLVar::get_chr() const 
    {
        if(VPtr == m_type)
            return m_u.m_ptr;
        assertType(SLVTStr);
        return m_u.m_buf;
    }

    void SLVar::assertType(SLVarType t) const
    {
        sl::assertType(t, type());
    }
    void SLVar::set_str(size_t len, const char* src)
    {
        if(len < maxBuf)
        {
            memset(m_u.m_buf, 0, maxBuf);
            strcpy_s(m_u.m_buf, maxBuf, src);
            m_type = SLVTStr;
        }
        else
        {
            m_u.m_ptr = new char[len + 1];
            strcpy_s(m_u.m_ptr, len + 1, src);
            m_type = VPtr;
        }
    }
}