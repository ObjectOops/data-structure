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

    // `ds::exception` is for internal use only.
    // Exception constructors expect memory allocated with `new`.
    // Destructors are solely responsible for deallocation.
    namespace exception {
        struct out_of_bounds {

            private:
            const char *msg;

            public:
            inline out_of_bounds(const char *msg) : msg {msg} {
            }
            inline ~out_of_bounds() {
                delete[] msg;
            }
            inline const char *what() {
                return msg;
            }
        };
    }

    struct str {

        private:
        char *s;
        ull n;
        
        public:
        inline str(const char *src) {
            this->n = strlen(src);
            this->s = new char [this->n + 1];
            memcpy(this->s, src, n);
            this->s[n] = '\0';
        }

        inline str(const str &obj) : str {obj.s} {
        }

        inline str(str &&obj) {
            this->s = obj.s;
            this->n = obj.n;
            obj.s = nullptr;
            obj.n = 0;
        }

        inline str() : str {""} {
        }

        inline ~str() {
            delete[] s;
        }

        inline ull length() const {
            return n;
        }

        inline str substr(ull start, ull end) const {
            start = start < n ? start : n;
            end = end < n ? end : n;
            ull len = end - start;
            char *buffer {new char [len + 1]};
            memcpy(buffer, s + start, len);
            buffer[len] = '\0';
            str ret {};
            delete[] ret.s;
            ret.s = buffer;
            ret.n = strlen(buffer);
            return ret;
        }

        inline const char *cstr() const {
            return s;
        }

        inline char &operator[](ull index) const {
            if (index >= n) {
                char *msg {new char [128]};
                snprintf(msg, 128, "ds::str subscript out of bounds exception with index %llu.", n);
                throw exception::out_of_bounds {msg};
            }
            return s[index];
        }

        inline str &operator=(const str &other) {
            delete[] s;
            s = new char [other.n + 1];
            memcpy(s, other.s, other.n);
            s[other.n] = '\0';
            n = other.n;
            return *this;
        }

        inline str operator+(const str &other) const {
            ull len = n + other.n;
            char *buffer {new char [len + 1]};
            memcpy(buffer, s, n);
            memcpy(buffer + n, other.s, other.n);
            buffer[len] = '\0';
            str ret {};
            delete[] ret.s;
            ret.s = buffer;
            ret.n = strlen(buffer);
            return ret;
        }

        inline str &operator+=(const str &other) {
            return operator=(operator+(other));
        }

        inline bool operator==(const str &other) {
            return strncmp(s, other.s, n < other.n ? other.n : n) == 0;
        }
    };

    template<typename Type>
    struct node {
        
        FUNC_TEMPLATE
        friend class structure;

        private:
        Type *p = nullptr;
        node *left = nullptr, *right = nullptr;
    };

    // The class templates require the default hash and comparison functions 
    // to take all parameters by reference, including primitive types.

    template<typename Type>
    inline ull default_hash(const Type &);

    template<typename Type>
    inline bool default_compare(const Type &, const Type &);

    template<typename T>
    struct _argv {

        template<typename Type, typename ...Types>
        friend _argv<Type> args(const Types &...);

        FUNC_TEMPLATE
        friend class structure;

        private:
        const T **v;
        ull n;
        _argv(const T **v, ull n) : v {v}, n {n} {
        }

        public:
        ~_argv() {
            delete[] v;
        }
    };
    // The returned pointer points to a block allocated by malloc so we can realloc it if needed later.
    template<typename Type, typename ...Types>
    inline _argv<Type> args(const Types &...argl) {
        ull bufferSize {sizeof...(Types)};

        // Type *initBuffer {new Type [bufferSize] {argl...}};

        const Type **initBuffer {new const Type* [bufferSize] {&argl...}};
        
        return _argv<Type> {initBuffer, bufferSize};
    }
    
    CLASS_TEMPLATE class structure {

        private:
        node<FirstType> *baseHead = nullptr, *baseTail = nullptr;
        ull size = 0;
        
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
        ull i {};
        if (argv.n > 0 && baseHead == nullptr) {
            baseHead = new node<FirstType> {};
            baseHead->p = new FirstType {*argv.v[0]};
            baseTail = baseHead;
            ++i;
        }
        for (; i < argv.n; ++i) {
            node<FirstType> *rightNode = new node<FirstType> {};
            rightNode->p = new FirstType {*argv.v[i]};
            rightNode->left = baseTail;
            baseTail->right = rightNode;
            baseTail = rightNode;
        }
        for (; i < n; ++i) {
            node<FirstType> *rightNode = new node<FirstType> {};
            rightNode->p = new FirstType {};
            rightNode->left = baseTail;
            baseTail->right = rightNode;
            baseTail = rightNode;
        }

        size = argv.n;
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::structure(ull n, const Hash &hash, const Compare &compare) {
        structure(args<FirstType>(), n, hash, compare);
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::~structure() {
    }

    template<typename Type>
    inline ull default_hash(const Type &value) {
        return value.hash();
    }
    // Modified K&R hash function.
    template<>
    inline ull default_hash<str>(const str &s) {
        ull slen {s.length()};
        ull len {slen / 8 + 1};
        ull *values {new ull [len] {}};
        memcpy(values, s.cstr(), slen);
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
        ull lhs_len {lhs.length()}, rhs_len {rhs.length()};
        // Assume that the null terminator will be present under normal circumstances, 
        // but mitigate the risk of buffer overflow.
        return strncmp(lhs.cstr(), rhs.cstr(), lhs_len < rhs_len ? rhs_len : lhs_len) < 0;
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
