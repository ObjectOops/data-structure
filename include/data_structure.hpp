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

    // `ds::exception` is for internal use only!
    namespace exception {

        struct base {

            protected:
            char *msg;

            public:
            base(
                const char *exceptionName = "ds::base unnamed exception", 
                const char *content = "A data_structure exception was thrown.", 
                bool selfDestruct = false, 
                const char *separator = ":\n\t"
            ) {
                ull size {strlen(exceptionName) + strlen(separator) + strlen(content) + 1};
                this->msg = new char [size];
                snprintf(this->msg, size, "%s%s%s", exceptionName, separator, content);
                
                // Delete content if allocated with new.
                // For convenience, may change later.
                if (selfDestruct) {
                    delete[] content;
                }
            }
            base(const base &other) {
                ull size {strlen(other.msg) + 1};
                this->msg = new char [size];
                memcpy(msg, other.msg, size);
            }
            base(base &&other) noexcept {
                this->msg = other.msg;
                other.msg = nullptr;
            }
            ~base() {
                delete[] msg;
            }
            inline const char *what() const noexcept {
                return msg;
            }
        };

        struct out_of_bounds : public base {

            public:
            out_of_bounds(const char *msg, bool selfDestruct = false) : 
                base {"ds out-of-bounds exception", msg, selfDestruct} 
            {
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

    class str {

        private:
        char *s;
        ull len;

        str(int) { // Nothing constructor.
        }
        
        public:
        str(const char *src) {
            this->len = strlen(src);
            this->s = new char [this->len + 1];
            memcpy(this->s, src, len);
            this->s[len] = '\0';
        }

        str(const str &obj) : str {obj.s} {
        }

        str(str &&obj) noexcept {
            this->s = obj.s;
            this->len = obj.len;
            obj.s = nullptr;
            obj.len = 0;
        }

        str() : str {""} {
        }

        ~str() {
            delete[] s;
        }

        inline ull length() const noexcept {
            return len;
        }

        str substr(ull start, ull end) const {
            start = start < len ? start : len;
            end = end < len ? end : len;
            ull len = end - start;
            char *buffer {new char [len + 1]};
            memcpy(buffer, s + start, len);
            buffer[len] = '\0';
            str ret {0};
            ret.s = buffer;
            ret.len = strlen(buffer);
            return ret;
        }

        inline const char *cstr() const noexcept {
            return s;
        }

        char &operator[](ull index) {
            if (index >= len) {
                ull size {128};
                char *msg {new char [size]};
                snprintf(
                    msg, 
                    size, 
                    "ds::str string access at index %llu with length %llu.", 
                    index, 
                    len
                );
                throw exception::out_of_bounds {msg, true};
            }
            return s[index];
        }

        str &operator=(const str &other) {
            delete[] s;
            s = new char [other.len + 1];
            memcpy(s, other.s, other.len);
            s[other.len] = '\0';
            len = other.len;
            return *this;
        }

        str operator+(const str &other) const {
            ull size = len + other.len;
            char *buffer {new char [size + 1]};
            memcpy(buffer, s, len);
            memcpy(buffer + len, other.s, other.len);
            buffer[size] = '\0';
            str ret {0};
            ret.s = buffer;
            ret.len = strlen(buffer);
            return ret;
        }

        str &operator+=(const str &other) {
            return operator=(operator+(other));
        }

        bool operator==(const str &other) const noexcept {
            return strncmp(s, other.s, len < other.len ? other.len : len) == 0;
        }
    };

    // The class templates require the default hash and comparison functions 
    // to take all parameters by reference, including primitive types.

    template<typename Type>
    ull default_hash(const Type &);

    template<typename Type>
    bool default_compare(const Type &, const Type &);

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
    _argv<Type> args(const Types &...argl) {
        ull bufferSize {sizeof...(Types)};

        // Type *initBuffer {new Type [bufferSize] {argl...}};

        const Type **initBuffer {new const Type* [bufferSize] {&argl...}};
        
        return _argv<Type> {initBuffer, bufferSize};
    }
    template<typename FirstType, typename SecondType, ull size>
    _argv<ipair<FirstType, SecondType>> args(const ipair<FirstType, SecondType> (&pairs) [size]) {
        const ipair<FirstType, SecondType> **initBuffer {new const ipair<FirstType, SecondType>* [size]};
        for (ull i {}; i < size; ++i) {
            initBuffer[i] = &pairs[i];
        }
        return _argv<ipair<FirstType, SecondType>> {initBuffer, size};
    }

    template<typename Type>
    struct _node {
        
        FUNC_TEMPLATE
        friend class structure;

        template<typename Type1, typename Type2>
        friend class pair;

        private:
        Type *p {nullptr};
        _node<Type> *left {nullptr}, *right {nullptr};
    };

    /*
     * Pseudo Plan
     * 
     * Need:
     * - Vector / Array y
     * - Deque y
     * - Linked List y
     * - Circular Linked List
     * - Set by Order y
     * - Multiset by Order y
     * - Map by Order y
     * - Multimap by Order y
     *      - Sets, maps, multisets, multimaps by hashing. y
     * - Stack y
     * - Queue y
     * - Priority Queue y
     * 
     * ipair<int, int> arguments [] {{1, 2}, {2, 3}, {3, 4}};
     * structure<int, int> test {args(arguments)};
     * 
     * test.random() --> Retrieves a pseudo-random pair. We don't need this...
     * test.foreach(lambda) --> For each loop.
     * 
     * test.pair[0] --> {1, 2} // Retrieves pair / iterator of FirstType and SecondType.
     * test.pair[1] --> {2, 3}
     * test.pair[2] --> {3, 4}
     * 
     * test.first[0] --> 1 // Retrieves FirstType.
     * test.first[1] --> 2
     * test.first[2] --> 3
     * 
     * test.second[0] --> 2 // Retrieves SecondType.
     * test.second[1] --> 3
     * test.second[2] --> 4
     * 
     * // The following really only operate on on FirstType.
     * 
     * test.ordered // Ordered map / set operations.
     * test.hashed // Hashed map / set operations.
     * 
     * test.deque // Deque operations.
     * test.stack // Stack operations.
     * test.queue // Queue operations.
     * test.pq // Priority queue operations.
     * 
     * ds::pair<int, int> // Pair which also doubles an an iterator?
     * 
     * To Do:
     * - Tests for iterator.
     * - May need some kind of state class to manage weird pair-structure interactions.
     *      - Use address of object to determine ownership!
     */

    template<typename Type1, typename Type2>
    class pair {

        FUNC_TEMPLATE
        friend class structure;

        private:
        _node<Type1> *n1;
        _node<Type2> *n2;

        bool ref; // true --> don't delete on destruction, false --> delete on destruction

        pair(_node<Type1> *n1, _node<Type2> *n2) : n1 {n1}, n2 {n2}, ref {true} {
        }

        public:
        pair(const Type1 &first, const Type2 &second) : ref {false} {
            n1 = new _node<Type1> {};
            n2 = new _node<Type2> {};
            n1->p = new Type1 {first};
            n2->p = new Type2 {second};
        }
        pair() : pair(Type1 {}, Type2 {}) {
        }
        pair(const pair<Type1, Type2> &other) : pair {other.n1, other.n2} {
        }
        ~pair() {
            if (!ref) {
                delete n1->p;
                delete n2->p;
                delete n1;
                delete n2;
            }
        }

        inline const Type1 &getFirst() const noexcept {
            return *(n1->p);
        }
        inline const Type2 &getSecond() const noexcept {
            return *(n2->p);
        }
    };
    
    CLASS_TEMPLATE class structure {

        private:
        _node<FirstType> *ft_baseHead {nullptr}, *ft_baseTail {nullptr};
        _node<SecondType> *st_baseHead {nullptr}, *st_baseTail {nullptr};
        ull size {};

        struct secondary {

            protected:
            structure<FirstType, SecondType, Hash, Compare> *primary;
        };

        struct linkedlist_secondary : public secondary {

            friend structure;
            
            private:
            _node<pair<FirstType, SecondType>> *head {nullptr}, *tail {nullptr};

            public:
            linkedlist_secondary(structure<FirstType, SecondType, Hash, Compare> *p) {
                secondary::primary = p;
            }
        };
        linkedlist_secondary linkedlist {this};

        template<typename _nodeType>
        inline void util_link(_nodeType tail, _nodeType newNode) {
            newNode->left = tail;
            tail->right = newNode;
            tail = newNode;
        }
        SecondType *util_initSecondType(bool forceInit) {
            return forceInit ? new SecondType {} : nullptr;
        }

        public:
        static bool forceSecondTypeInit;

        Hash hash;
        Compare compare;

        structure(_argv<FirstType> = args<FirstType>(), ull = 0, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(_argv<ipair<FirstType, SecondType>>, ull = 0, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(ull, const Hash &hash = default_hash, const Compare &compare = default_compare);
        ~structure();
    };

    FUNC_TEMPLATE bool structure<FirstType, SecondType, Hash, Compare>::forceSecondTypeInit = false;

    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::structure(
        _argv<FirstType> argv, 
        ull n, 
        const Hash &hash, 
        const Compare &compare
    ) : 
        hash {hash}, compare {compare} 
    {
        ull i {};
        if (argv.n > 0 && ft_baseHead == nullptr) {
            ft_baseHead = new _node<FirstType> {};
            ft_baseHead->p = new FirstType {*argv.v[0]};
            ft_baseTail = ft_baseHead;

            st_baseHead = new _node<SecondType> {};
            st_baseHead->p = util_initSecondType(forceSecondTypeInit);
            st_baseTail = st_baseHead;

            linkedlist.head = new _node<pair<FirstType, SecondType>> {};
            linkedlist.head->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
            linkedlist.tail = linkedlist.head;

            ++i;
        }
        for (; i < argv.n; ++i) {
            _node<FirstType> *rightNode {new _node<FirstType> {}};
            rightNode->p = new FirstType {*argv.v[i]};
            util_link(ft_baseTail, rightNode);

            _node<SecondType> *rightNodeSecond {new _node<SecondType> {}};
            rightNodeSecond->p = util_initSecondType(forceSecondTypeInit);
            util_link(st_baseTail, rightNodeSecond);

            _node<pair<FirstType, SecondType>> *ll_node {new _node<pair<FirstType, SecondType>> {}};
            ll_node->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
            util_link(linkedlist.tail, ll_node);
        }
        for (; i < n; ++i) {
            _node<FirstType> *rightNode {new _node<FirstType> {}};
            rightNode->p = new FirstType {};
            util_link(ft_baseTail, rightNode);

            _node<SecondType> *rightNodeSecond {new _node<SecondType> {}};
            rightNodeSecond->p = util_initSecondType(forceSecondTypeInit);
            util_link(st_baseTail, rightNodeSecond);

            _node<pair<FirstType, SecondType>> *ll_node {new _node<pair<FirstType, SecondType>> {}};
            ll_node->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
            util_link(linkedlist.tail, ll_node);
        }

        size = argv.n;
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::structure(
        _argv<ipair<FirstType, SecondType>> argv, 
        ull n, 
        const Hash &hash, 
        const Compare &compare
    ) : 
        hash {hash}, compare {compare} 
    {
        ull i {};
        if (argv.n > 0 && ft_baseHead == nullptr) {
            ft_baseHead = new _node<FirstType> {};
            ft_baseHead->p = new FirstType {*argv.v[0]->first};
            ft_baseTail = ft_baseHead;

            st_baseHead = new _node<SecondType> {};
            st_baseHead->p = new SecondType {*argv.v[0]->second};
            st_baseTail = st_baseHead;

            linkedlist.head = new _node<pair<FirstType, SecondType>> {};
            linkedlist.head->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
            linkedlist.tail = linkedlist.head;

            ++i;
        }
        for (; i < argv.n; ++i) {
            _node<FirstType> *rightNode {new _node<FirstType> {}};
            rightNode->p = new FirstType {*argv.v[i]->first};
            util_link(ft_baseTail, rightNode);

            _node<SecondType> *rightNodeSecond {new _node<SecondType> {}};
            rightNodeSecond->p = new SecondType {*argv.v[i]->second};
            util_link(st_baseTail, rightNodeSecond);

            _node<pair<FirstType, SecondType>> *ll_node {new _node<pair<FirstType, SecondType>> {}};
            ll_node->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
            util_link(linkedlist.tail, ll_node);
        }
        for (; i < n; ++i) {
            _node<FirstType> *rightNode {new _node<FirstType> {}};
            rightNode->p = new FirstType {};
            util_link(ft_baseTail, rightNode);

            _node<SecondType> *rightNodeSecond {new _node<SecondType> {}};
            rightNodeSecond->p = util_initSecondType(forceSecondTypeInit);
            util_link(st_baseTail, rightNodeSecond);

            _node<pair<FirstType, SecondType>> *ll_node {new _node<pair<FirstType, SecondType>> {}};
            ll_node->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
            util_link(linkedlist.tail, ll_node);
        }

        size = argv.n;
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::structure(
        ull n, 
        const Hash &hash, 
        const Compare &compare
    ) {
        structure(args<FirstType>(), n, hash, compare);
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare>::~structure() {
        _node<FirstType> *ft_dnode {ft_baseHead};
        while (ft_dnode != nullptr) {
            delete ft_dnode->p;
            _node<FirstType> *tnode {ft_dnode};
            ft_dnode = ft_dnode->right;
            delete tnode;
        }

        _node<SecondType> *st_dnode {st_baseHead};
        while (st_dnode != nullptr) {
            delete st_dnode->p;
            _node<SecondType> *tnode {st_dnode};
            st_dnode = st_dnode->right;
            delete tnode;
        }

        _node<pair<FirstType, SecondType>> *ll_dnode {linkedlist.head};
        while (ll_dnode != nullptr) {
            delete ll_dnode->p->n1;
            delete ll_dnode->p->n2;
            delete ll_dnode->p;
            _node<pair<FirstType, SecondType>> *tnode {ll_dnode};
            ll_dnode = ll_dnode->right;
            delete tnode;
        }
    }

    template<typename Type>
    ull default_hash(const Type &value) {
        return value.hash();
    }
    // Modified K&R hash function.
    template<>
    ull default_hash<str>(const str &s) {
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
    bool default_compare(const Type &lhs, const Type &rhs) {
        return lhs < rhs;
    }
    template<>
    bool default_compare<str>(const str &lhs, const str &rhs) {
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
