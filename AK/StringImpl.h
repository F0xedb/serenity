#pragma once

#include <AK/RefCounted.h>
#include <AK/RefPtr.h>
#include <AK/Types.h>
#include <AK/kmalloc.h>

namespace AK {

enum ShouldChomp {
    NoChomp,
    Chomp
};

class StringImpl : public RefCounted<StringImpl> {
public:
    static NonnullRefPtr<StringImpl> create_uninitialized(int length, char*& buffer);
    static RefPtr<StringImpl> create(const char* cstring, ShouldChomp = NoChomp);
    static RefPtr<StringImpl> create(const char* cstring, int length, ShouldChomp = NoChomp);
    NonnullRefPtr<StringImpl> to_lowercase() const;
    NonnullRefPtr<StringImpl> to_uppercase() const;

    void operator delete(void* ptr)
    {
        kfree(ptr);
    }

    static StringImpl& the_empty_stringimpl();

    ~StringImpl();

    int length() const {
        return m_length;
    }
    const char* characters() const {
        return &m_inline_buffer[0];
    }
    char operator[](int i) const
    {
        ASSERT(i >= 0 && i < m_length);
        return characters()[i];
    }

    unsigned hash() const
    {
        if (!m_has_hash)
            compute_hash();
        return m_hash;
    }

    NonnullRefPtr<StringImpl> reversed() const;

private:
    enum ConstructTheEmptyStringImplTag {
        ConstructTheEmptyStringImpl
    };
    explicit StringImpl(ConstructTheEmptyStringImplTag)
    {
        m_inline_buffer[0] = '\0';
    }

    enum ConstructWithInlineBufferTag {
        ConstructWithInlineBuffer
    };
    StringImpl(ConstructWithInlineBufferTag, int length);

    void compute_hash() const;

    int m_length { 0 };
    mutable unsigned m_hash { 0 };
    mutable bool m_has_hash { false };
    char m_inline_buffer[0];
};

inline constexpr u32 string_hash(const char* characters, int length)
{
    u32 hash = 0;
    for (int i = 0; i < length; ++i) {
        hash += (u32)characters[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

}

using AK::Chomp;
using AK::string_hash;
using AK::StringImpl;
