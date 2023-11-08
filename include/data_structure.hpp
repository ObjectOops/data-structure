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

    namespace util {
        template<typename Type, typename OtherType>
        struct same_type {
            static constexpr bool result = false;
        };
        template<typename Type>
        struct same_type<Type, Type> {
            static constexpr bool result = true;
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

    // The class templates require the default hash and comparison functions 
    // to take all parameters by reference, including primitive types.

    template<typename Type>
    inline ull default_hash(const Type &);

    template<typename Type>
    inline bool default_compare(const Type &, const Type &);

    template<typename T1, typename T2>
    struct ipair {

        FUNC_TEMPLATE
        friend class structure;

        private:
        const T1 *first;
        const T2 *second;

        public:
        ipair(const T1 &first, const T2 &second) : 
            first {&first}, second {&second} 
        {
        }
    };

    template<typename T>
    struct _argv {

        template<typename Type, typename ...Types>
        friend _argv<Type> args(const Types &...);
        
        template<typename FirstType, typename SecondType, ull size>
        friend _argv<ipair<FirstType, SecondType>> args(const ipair<FirstType, SecondType> (&) [size]);

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
    template<typename Type, typename ...Types>
    inline _argv<Type> args(const Types &...argl) {
        ull bufferSize {sizeof...(Types)};

        // Type *initBuffer {new Type [bufferSize] {argl...}};

        const Type **initBuffer {new const Type* [bufferSize] {&argl...}};
        
        return _argv<Type> {initBuffer, bufferSize};
    }
    template<typename FirstType, typename SecondType, ull size>
    inline _argv<ipair<FirstType, SecondType>> args(const ipair<FirstType, SecondType> (&pairs) [size]) {
        const ipair<FirstType, SecondType> **initBuffer {new const ipair<FirstType, SecondType>* [size]};
        for (ull i {}; i < size; ++i) {
            initBuffer[i] = &pairs[i];
        }
        return _argv<ipair<FirstType, SecondType>> {initBuffer, size};
    }

    template<typename Type, typename PartnerType>
    struct _node {
        
        FUNC_TEMPLATE
        friend class structure;

        private:
        Type *p = nullptr;
        _node<Type, PartnerType> *left = nullptr, *right = nullptr;
        _node<PartnerType, Type> *partner = nullptr;
    };
    
    CLASS_TEMPLATE class structure {

        private:
        _node<FirstType, SecondType> *ft_baseHead = nullptr, *ft_baseTail = nullptr;
        _node<SecondType, FirstType> *st_baseHead = nullptr, *st_baseTail = nullptr;
        ull size = 0;
        
        public:
        Hash hash;
        Compare compare;

        structure(_argv<FirstType> = args<FirstType>(), ull = 0, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(_argv<ipair<FirstType, SecondType>>, ull = 0, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(ull, const Hash &hash = default_hash, const Compare &compare = default_compare);
        ~structure();
    };

    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::structure(_argv<FirstType> argv, ull n, const Hash &hash, const Compare &compare) : 
        hash {hash}, compare {compare} 
    {
        ull i {};
        if (argv.n > 0 && ft_baseHead == nullptr) {
            ft_baseHead = new _node<FirstType, SecondType> {};
            ft_baseHead->p = new FirstType {*argv.v[0]};
            ft_baseTail = ft_baseHead;

            st_baseHead = new _node<SecondType, FirstType> {};
            st_baseHead->p = new SecondType {};
            st_baseTail = st_baseHead;

            ft_baseHead->partner = st_baseHead;
            st_baseHead->partner = ft_baseHead;

            ++i;
        }
        for (; i < argv.n; ++i) {
            _node<FirstType, SecondType> *rightNode {new _node<FirstType, SecondType> {}};
            rightNode->p = new FirstType {*argv.v[i]};
            rightNode->left = ft_baseTail;
            ft_baseTail->right = rightNode;
            ft_baseTail = rightNode;

            _node<SecondType, FirstType> *rightNodeSecond {new _node<SecondType, FirstType> {}};
            rightNodeSecond->p = new SecondType {};
            rightNodeSecond->left = st_baseTail;
            st_baseTail->right = rightNodeSecond;
            st_baseTail = rightNodeSecond;

            ft_baseTail->partner = st_baseTail;
            st_baseTail->partner = ft_baseTail;
        }
        for (; i < n; ++i) {
            _node<FirstType, SecondType> *rightNode {new _node<FirstType, SecondType> {}};
            rightNode->p = new FirstType {};
            rightNode->left = ft_baseTail;
            ft_baseTail->right = rightNode;
            ft_baseTail = rightNode;

            _node<SecondType, FirstType> *rightNodeSecond {new _node<SecondType, FirstType> {}};
            rightNodeSecond->p = new SecondType {};
            rightNodeSecond->left = st_baseTail;
            st_baseTail->right = rightNodeSecond;
            st_baseTail = rightNodeSecond;

            ft_baseTail->partner = st_baseTail;
            st_baseTail->partner = ft_baseTail;
        }

        size = argv.n;
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::structure(_argv<ipair<FirstType, SecondType>> argv, ull n, const Hash &hash, const Compare &compare) : 
        hash {hash}, compare {compare} 
    {
        ull i {};
        if (argv.n > 0 && ft_baseHead == nullptr) {
            ft_baseHead = new _node<FirstType, SecondType> {};
            ft_baseHead->p = new FirstType {*argv.v[0]->first};
            ft_baseTail = ft_baseHead;

            st_baseHead = new _node<SecondType, FirstType> {};
            st_baseHead->p = new SecondType {*argv.v[0]->second};
            st_baseTail = st_baseHead;

            ft_baseHead->partner = st_baseHead;
            st_baseHead->partner = ft_baseHead;

            ++i;
        }
        for (; i < argv.n; ++i) {
            _node<FirstType, SecondType> *rightNode {new _node<FirstType, SecondType> {}};
            rightNode->p = new FirstType {*argv.v[i]->first};
            rightNode->left = ft_baseTail;
            ft_baseTail->right = rightNode;
            ft_baseTail = rightNode;

            _node<SecondType, FirstType> *rightNodeSecond {new _node<SecondType, FirstType> {}};
            rightNodeSecond->p = new SecondType {*argv.v[i]->second};
            rightNodeSecond->left = st_baseTail;
            st_baseTail->right = rightNodeSecond;
            st_baseTail = rightNodeSecond;

            ft_baseTail->partner = st_baseTail;
            st_baseTail->partner = ft_baseTail;
        }
        for (; i < n; ++i) {
            _node<FirstType, SecondType> *rightNode {new _node<FirstType, SecondType> {}};
            rightNode->p = new FirstType {};
            rightNode->left = ft_baseTail;
            ft_baseTail->right = rightNode;
            ft_baseTail = rightNode;

            _node<SecondType, FirstType> *rightNodeSecond {new _node<SecondType, FirstType> {}};
            rightNodeSecond->p = new SecondType {};
            rightNodeSecond->left = st_baseTail;
            st_baseTail->right = rightNodeSecond;
            st_baseTail = rightNodeSecond;

            ft_baseTail->partner = st_baseTail;
            st_baseTail->partner = ft_baseTail;
        }

        size = argv.n;
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::structure(ull n, const Hash &hash, const Compare &compare) {
        structure(args<FirstType>(), n, hash, compare);
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::~structure() {
        _node<FirstType, SecondType> *ft_dnode {ft_baseHead};
        while (ft_dnode != ft_baseTail) {
            delete ft_dnode->p;
            _node<FirstType, SecondType> *tnode {ft_dnode};
            ft_dnode = ft_dnode->right;
            delete tnode;
        }

        _node<SecondType, FirstType> *st_dnode {st_baseHead};
        while (st_dnode != st_baseTail) {
            delete st_dnode->p;
            _node<SecondType, FirstType> *tnode {st_dnode};
            st_dnode = st_dnode->right;
            delete tnode;
        }
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
