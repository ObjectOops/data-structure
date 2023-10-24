#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include <string.h>

#define CLASS_TEMPLATE \
template<\
    typename FirstType, \
    typename SecondType = int, \
    typename Hash = ull (*) (const FirstType &), \
    typename Compare = bool (*) (const FirstType &, const FirstType &), \
    int N = -1\
>

#define FUNC_TEMPLATE \
template<\
    typename FirstType, \
    typename SecondType, \
    typename Hash, \
    typename Compare, \
    int N\
>

#define DEFAULT_HASH(PT) \
template<> \
inline ull default_hash<PT>(const PT &value) { \
    return value; \
}

#define DEFAULT_COMPARE(PT) \
template<> \
inline bool default_compare<PT>(const PT &lhs, const PT &rhs) { \
    return lhs < rhs; \
}

namespace ds {

    typedef unsigned long long ull;

    // The class templates require the default hash and comparison functions 
    // to take all parameters by reference, including primitive types.

    template<typename Type>
    inline ull default_hash(const Type &);

    template<typename Type>
    inline bool default_compare(const Type &, const Type &);

    struct str {
        char *p;
        ull n;
    };
    
    CLASS_TEMPLATE class structure {

        private:
        
        public:
        Hash hash;
        Compare compare;

        structure(const Hash &hash = default_hash, const Compare &compare = default_compare);
    };

    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare, N>::structure(const Hash &hash, const Compare &compare) : 
        hash {hash}, compare {compare} 
    {}

    template<typename Type>
    inline ull default_hash(const Type &value) {
        return value.hash();
    }
    // Modified K&R hash function.
    template<>
    inline ull default_hash<str>(const str &s) {
        ull len = s.n / 8 + 1;
        ull *values {new ull [len]};
        memcpy(values, s.p, s.n);
        ull h {};
        for (ull i {}; i < len; ++i) {
            h *= 31;
            h += values[i];
        }
        return h;
    }
    DEFAULT_HASH(short)
    DEFAULT_HASH(int)
    DEFAULT_HASH(long)
    DEFAULT_HASH(long long)
    DEFAULT_HASH(unsigned short)
    DEFAULT_HASH(unsigned int)
    DEFAULT_HASH(unsigned long)
    DEFAULT_HASH(ull)
    DEFAULT_HASH(char)
    DEFAULT_HASH(wchar_t)
    DEFAULT_HASH(char16_t)
    DEFAULT_HASH(char32_t)
    DEFAULT_HASH(bool)

    template<typename Type>
    inline bool default_compare(const Type &lhs, const Type &rhs) {
        return lhs < rhs;
    }
    template<>
    inline bool default_compare<str>(const str &lhs, const str &rhs) {
        return strncmp(lhs.p, rhs.p, lhs.n < rhs.n ? lhs.n : rhs.n);
    }
    DEFAULT_COMPARE(short)
    DEFAULT_COMPARE(int)
    DEFAULT_COMPARE(long)
    DEFAULT_COMPARE(long long)
    DEFAULT_COMPARE(unsigned short)
    DEFAULT_COMPARE(unsigned int)
    DEFAULT_COMPARE(unsigned long)
    DEFAULT_COMPARE(ull)
    DEFAULT_COMPARE(char)
    DEFAULT_COMPARE(wchar_t)
    DEFAULT_COMPARE(char16_t)
    DEFAULT_COMPARE(char32_t)
    DEFAULT_COMPARE(bool)
}

#undef CLASS_TEMPLATE
#undef FUNC_TEMPLATE
#undef DEFAULT_HASH
#undef DEFAULT_COMPARE

#endif
