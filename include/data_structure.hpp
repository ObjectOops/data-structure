#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include <string.h>
#include <malloc.h>

#define dhash_t(T) ull (*) (const T &)
#define dcomp_t(T) bool (*) (const T &, const T &)

#define CLASS_TEMPLATE \
template<\
    typename FirstType, \
    typename SecondType = int, \
    typename Hash = dhash_t(FirstType), \
    typename Compare = dcomp_t(FirstType) \
>

#define FUNC_TEMPLATE \
template<\
    typename FirstType, \
    typename SecondType, \
    typename Hash, \
    typename Compare \
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
        char *s;
        ull n;
        
        inline str(const char *src, ull n) {
            this->s = new char [n + 1];
            this->n = n;
            snprintf(this->s, n + 1, src);
        }

        inline str(const str &obj) {
            str(obj.s, obj.n);
        }

        inline str(const str &&obj) {
            str(obj.s, obj.n);
        }

        inline str() {
            str("", 0);
        }

        inline ~str() {
            delete[] s;
        }

        inline bool operator<(const str &other) {
            return strncmp(this->s, other.s, this->n < other.n ? this->n : other.n) < 0;
        }
    };

    template<typename T>
    struct _argv {

        template<typename Type, typename ...Types>
        friend _argv<Type> args(const Types &...);

        FUNC_TEMPLATE
        friend class structure;

        private:
        T *v;
        ull n;
        _argv(T *v, ull n) : v {v}, n {n} {
        }
    };
    // The returned pointer points to a block allocated by malloc so we can realloc it if needed later.
    template<typename Type, typename ...Types>
    inline _argv<Type> args(const Types &...argl) {
        ull typeSize {sizeof(Type)}, bufferSize {sizeof...(Types)};
        Type *p {reinterpret_cast<Type *>(malloc(typeSize * bufferSize))};
        Type *initBuffer {new Type [bufferSize] {argl...}};
        for (ull i {}; i < bufferSize; ++i) {
            p[i] = Type (initBuffer[i]);
        }
        delete[] initBuffer;
        return _argv<Type> {p, bufferSize};
    }
    
    CLASS_TEMPLATE class structure {

        private:
        FirstType *placeholder;
        ull pl2;
        
        public:
        Hash hash;
        Compare compare;

        structure(_argv<FirstType> = args<FirstType>(), ull = 0, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(ull, const Hash &hash = default_hash, const Compare &compare = default_compare);
        ~structure();
    };

    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::structure(_argv<FirstType> argv, ull n, const Hash &hash, const Compare &compare) : 
        hash {hash}, compare {compare} 
    {
        placeholder = argv.v;
        pl2 = n;
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::structure(ull n, const Hash &hash, const Compare &compare) {
        structure(args<FirstType>(), n, hash, compare);
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::~structure() {
        free(placeholder);
    }

    template<typename Type>
    inline ull default_hash(const Type &value) {
        return value.hash();
    }
    // Modified K&R hash function.
    template<>
    inline ull default_hash<str>(const str &s) {
        ull len = s.n / 8 + 1;
        ull *values {new ull [len]};
        memcpy(values, s.s, s.n);
        ull h {};
        for (ull i {}; i < len; ++i) {
            h *= 31;
            h += values[i];
        }
        delete[] values;
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
        return strncmp(lhs.s, rhs.s, lhs.n < rhs.n ? lhs.n : rhs.n) < 0;
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
