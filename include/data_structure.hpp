#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include <string.h>
#include <malloc.h>

// Specifies to use a function pointer to the hash function for type `T`.
#define dhash_t(T) ull (*) (const T &)
// Specifies to use a function pointer to the compare function for type `T`.
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

// `DEFAULT_HASH` and `DEFAULT_COMPARE` are used internally to create default definitions 
// for primitive types `PT`.

#define DEFAULT_HASH(PT) \
template<> \
ull default_hash<PT>(const PT &value) { \
    return value; \
}

#define DEFAULT_COMPARE(PT) \
template<> \
bool default_compare<PT>(const PT &lhs, const PT &rhs) { \
    return lhs < rhs; \
}

namespace ds {

    typedef unsigned long long ull;

    // Extra utilities.
    namespace util {

        // Indicates that `Type` and `OtherType` are not the same at compile-time.
        template<typename Type, typename OtherType>
        struct same_type {
            static constexpr bool result = false;
        };
        // Indicates that `Type` and `Type` are the same at compile-time.
        template<typename Type>
        struct same_type<Type, Type> {
            static constexpr bool result = true;
        };
    }

    // Throwing exceptions from `ds::exception` are for internal use only!
    namespace exception {

        // Base exception.
        struct base {

            protected:
            char *msg;

            public:
            base(
                const char *exceptionName = "ds::base unnamed exception", 
                const char *content = "An exception was thrown.", 
                bool selfDestruct = false, 
                const char *separator = ":\n\t"
            ) {
                ull size {strlen(exceptionName) + strlen(separator) + strlen(content) + 1};
                this->msg = new char [size];
                snprintf(this->msg, size, "%s%s%s", exceptionName, separator, content);
                
                // Delete `content` if allocated with new. May change later.
                if (selfDestruct) {
                    delete[] content;
                }
            }
            base(const base &other) {
                ull size {strlen(other.msg) + 1};
                this->msg = new char [size];
                memcpy(msg, other.msg, size);
                this->msg[size - 1] = '\0';
            }
            base(base &&other) noexcept {
                this->msg = other.msg;
                other.msg = nullptr;
            }
            ~base() {
                delete[] msg;
            }

            const char *what() const noexcept {
                return msg;
            }
        };

        // Out-of-bounds exception.
        struct out_of_bounds : public base {

            public:
            out_of_bounds(const char *msg, bool selfDestruct = false) : 
                base {"ds::out_of_bounds exception", msg, selfDestruct} 
            {
            }
        };

        // Null / invalid data access exception.
        struct null_access : public base {

            public:
            null_access(const char *msg) : 
                base {"ds::null_access exception", msg, false} 
            {
            }
        };
    }

    // Basic string class.
    class str {

        private:
        char *s;
        ull len;

        // Constructor for internal use.
        str(char *s, ull len) : s {s}, len {len} {
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

        ull length() const noexcept {
            return len;
        }

        str substr(ull start, ull end) const {
            start = start < len ? start : len;
            end = end < len ? end : len;
            ull len = end - start;

            char *buffer {new char [len + 1]};
            memcpy(buffer, s + start, len);
            buffer[len] = '\0';

            return str {buffer, len};
        }

        const char *cstr() const noexcept {
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
        char operator[](ull index) const {
            return const_cast<str *>(this)->operator[](index);
        }

        str &operator=(const str &other) {
            delete[] s;
            s = new char [other.len + 1];
            memcpy(s, other.s, other.len);
            s[other.len] = '\0';
            len = other.len;
            return *this;
        }
        str &operator=(str &&other) {
            delete[] this->s;
            this->s = other.s;
            this->len = other.len;
            other.s = nullptr;
            other.len = 0;
            return *this;
        }

        str operator+(const str &other) const {
            ull size = len + other.len + 1;
            char *buffer {new char [size]};

            memcpy(buffer, s, len);
            memcpy(buffer + len, other.s, other.len);
            buffer[size - 1] = '\0';

            return str {buffer, size - 1};
        }

        str &operator+=(const str &other) {
            return operator=(operator+(other));
        }

        bool operator==(const str &other) const noexcept {
            return strncmp(s, other.s, len < other.len ? other.len : len) == 0;
        }
    };

    // Prototypes.
    // The class templates require the default hash and comparison functions 
    // to take all parameters by reference, including primitive types.

    template<typename Type>
    ull default_hash(const Type &);

    template<typename Type>
    bool default_compare(const Type &, const Type &);

    // Initialization pair. Make an array of these to initialize values to pass to `args`.
    template<typename T1, typename T2>
    struct ipair {

        FUNC_TEMPLATE
        friend class structure;

        private:
        T1 *first;
        T2 *second;

        public:
        ipair(const T1 &first, const T2 &second) : 
            first {new T1 {first}}, second {new T2 {second}} 
        {
        }
    };

    // Internal use only! For passing results from `args` to `structure` constructors.
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

    // Array-style initialization.
    template<typename Type, typename ...Types>
    _argv<Type> args(const Types &...argl) {
        ull bufferSize {sizeof...(Types)};

        // Type *initBuffer {new Type [bufferSize] {argl...}};

        const Type **initBuffer {new const Type* [bufferSize] {&argl...}};
        
        return _argv<Type> {initBuffer, bufferSize};
    }
    // Key-value / pair initialization.
    template<typename FirstType, typename SecondType, ull size>
    _argv<ipair<FirstType, SecondType>> args(const ipair<FirstType, SecondType> (&pairs) [size]) {
        const ipair<FirstType, SecondType> **initBuffer {new const ipair<FirstType, SecondType>* [size]};
        for (ull i {}; i < size; ++i) {
            initBuffer[i] = &pairs[i];
        }
        return _argv<ipair<FirstType, SecondType>> {initBuffer, size};
    }

    // Internally used node type for storing data in linked lists.
    template<typename Type>
    struct _node {
        
        FUNC_TEMPLATE
        friend class structure;

        template<typename Type1, typename Type2>
        friend class pair;

        private:
        Type *p {nullptr};
        _node<Type> *left {nullptr}, *right {nullptr};

        _node() {
        }
    };

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
        pair(const Type1 &first) : ref {false} {
            n1 = new _node<Type1> {};
            n2 = new _node<Type2> {};
            n1->p = new Type1 {first};
            n2->p = nullptr;
        }
        pair() : pair(Type1 {}, Type2 {}) {
        }
        pair(const pair<Type1, Type2> &other) : pair {*other.n1->p, *other.n2->p} {
        }
        pair(const pair<Type1, Type2> &&other) {
            this->n1 = other.n1;
            this->n2 = other.n2;
            other.n1 = nullptr;
            other.n2 = nullptr;
        }
        ~pair() {
            if (!ref) {
                delete n1->p;
                delete n2->p;
                delete n1;
                delete n2;
            }
        }

        Type1 &getFirst() noexcept {
            return *n1->p;
        }
        const Type1 &getFirst() const noexcept {
            return const_cast<pair<Type1, Type2> *>(this)->getFirst();
        }
        Type2 &getSecond() {
            if (n2->p == nullptr) {
                throw exception::null_access {
                    "Second value in pair is not initialized. this->getSecond() is invalid."
                };
            }
            return *n2->p;
        }
        const Type2 &getSecond() const {
            return const_cast<pair<Type1, Type2> *>(this)->getSecond();
        }

        bool secondExists() const noexcept {
            return n2->p != nullptr;
        }
    };
    
    CLASS_TEMPLATE class structure {

        private:
        _node<FirstType> *ft_baseHead {nullptr}, *ft_baseTail {nullptr};
        _node<SecondType> *st_baseHead {nullptr}, *st_baseTail {nullptr};
        ull size {};

        struct _cmd {
            enum TYPE {INSERT, REMOVE};
            TYPE type;
            ull data;
        };
        _node<_cmd> *cmd_head {nullptr}, *cmd_tail {nullptr};

        struct secondary {

            protected:
            structure<FirstType, SecondType, Hash, Compare> *primary;
        };

        public:
        class iterator; // Class prototype;
        private:

        struct linkedlist_secondary : public secondary {

            friend structure;
            
            private:
            _node<pair<FirstType, SecondType>> *head {nullptr};
            _node<pair<FirstType, SecondType>> *tail {new _node<pair<FirstType, SecondType>> {}};

            linkedlist_secondary(structure<FirstType, SecondType, Hash, Compare> *p) {
                secondary::primary = p;
            }

            public:
            iterator begin() const {
                return iterator {secondary::primary, head};
            }

            iterator end() const {
                return iterator {secondary::primary, tail};
            }
        };

        template<typename _nodeType>
        void util_link(_nodeType *&tail, _nodeType *&newNode) { // `tail` must be by reference.
            newNode->left = tail;
            tail->right = newNode;
            tail = newNode;
        }
        void util_link_pair(
            _node<pair<FirstType, SecondType>> *tail, 
            _node<pair<FirstType, SecondType>> *newNode)
        {
            tail->left->right = newNode;
            newNode->left = tail->left;
            newNode->right = tail;
            tail->left = newNode;
        }
        SecondType *util_initSecondType(bool forceInit) {
            return forceInit ? new SecondType {} : nullptr;
        }

        public:
        Hash hash;
        Compare compare;

        bool forceSecondTypeInit {false};

        linkedlist_secondary linkedlist {this};

        structure(_argv<FirstType> = args<FirstType>(), ull = 0, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(_argv<ipair<FirstType, SecondType>>, ull = 0, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(ull, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(const structure<FirstType, SecondType, Hash, Compare> &other);
        structure(const structure<FirstType, SecondType, Hash, Compare> &&other) noexcept;
        ~structure();

        class iterator {

            friend linkedlist_secondary;

            private:
            structure<FirstType, SecondType, Hash, Compare> *primary;
            _node<pair<FirstType, SecondType>> *p;

            iterator(
                structure<FirstType, SecondType, Hash, Compare> *primary, 
                _node<pair<FirstType, SecondType>> *p
            ) : primary {primary}, p {p} 
            {
            }

            void test_ptr(const void *ptr) const {
                if (ptr == nullptr) {
                    throw exception::null_access {"Iterator references null."};
                }
            }

            public:
            iterator() : iterator {nullptr, nullptr} {
            }
            iterator(const iterator &other) : iterator {other.primary, other.p} {
            }
            iterator &operator=(const iterator &other) noexcept {
                this->primary = other.primary;
                this->p = other.p;
                return *this;
            }
            bool operator==(const iterator &other) noexcept {
                return this->p == other.p;
            }
            bool valid() const {
                return p != nullptr && p->p != nullptr;
            }

            pair<FirstType, SecondType> &value() const {
                test_ptr(p);
                return *p->p;
            }
            pair<FirstType, SecondType> &operator*() const {
                test_ptr(p);
                return *p->p;
            }

            bool hasNext() {
                test_ptr(p);
                return p->right != nullptr;
            }
            bool hasPrev() {
                test_ptr(p);
                return p->left != nullptr;
            }

            pair<FirstType, SecondType> &next() {
                test_ptr(p);
                pair<FirstType, SecondType> &ret {*p->p};
                p = p->right;
                if (p == nullptr) {
                    throw exception::out_of_bounds {
                        R"(ds::structure::iterator cannot advance to next element. 
    Next element does not exist. this->next() is invalid.)"
                    };
                }
                return ret;
            }
            iterator &operator++() { // Pre-increment.
                test_ptr(p);
                p = p->right;
                if (p == nullptr) {
                    throw exception::out_of_bounds {
                        R"(ds::structure::iterator pre-increment failed. 
    Next element does not exist. ++(*this) is invalid.)"
                    };
                }
                return *this;
            }
            iterator &operator++(int) { // Post-increment.
                test_ptr(p);
                iterator &ret {*this};
                p = p->right;
                if (p == nullptr) {
                    throw exception::out_of_bounds {
                        R"(ds::structure::iterator post-increment failed. 
    Next element does not exist. (*this)++ is invalid.)"
                    };
                }
                return ret;
            }

            pair<FirstType, SecondType> &prev() {
                test_ptr(p);
                pair<FirstType, SecondType> &ret {*p->p};
                p = p->left;
                if (p == nullptr) {
                    throw exception::out_of_bounds {
                        R"(ds::structure::iterator cannot advance to previous element. 
    Previous element does not exist. this->prev() is invalid.)"
                    };
                }
                return ret;
            }

            iterator &operator--() { // Pre-decrement.
                test_ptr(p);
                p = p->left;
                if (p == nullptr) {
                    throw exception::out_of_bounds {
                        R"(ds::structure::iterator pre-decrement failed. 
    Previous element does not exist. --(*this) is invalid.)"
                    };
                }
                return *this;
            }
            iterator &operator--(int) { // Post-decrement.
                test_ptr(p);
                iterator &ret {*this};
                p = p->left;
                if (p == nullptr) {
                    throw exception::out_of_bounds {
                        R"(ds::structure::iterator post-decrement failed. 
    Previous element does not exist. (*this)-- is invalid.)"
                    };
                }
                return ret;
            }
        };
    };

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
            linkedlist.tail->left = linkedlist.head;

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
            util_link_pair(linkedlist.tail, ll_node);
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
            util_link_pair(linkedlist.tail, ll_node);
        }

        size = argv.n > n ? argv.n : n;
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
            ft_baseHead->p = argv.v[0]->first;
            ft_baseTail = ft_baseHead;

            st_baseHead = new _node<SecondType> {};
            st_baseHead->p = argv.v[0]->second;
            st_baseTail = st_baseHead;

            linkedlist.head = new _node<pair<FirstType, SecondType>> {};
            linkedlist.head->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
            linkedlist.tail->left = linkedlist.head;

            ++i;
        }
        for (; i < argv.n; ++i) {
            _node<FirstType> *rightNode {new _node<FirstType> {}};
            rightNode->p = argv.v[i]->first;
            util_link(ft_baseTail, rightNode);

            _node<SecondType> *rightNodeSecond {new _node<SecondType> {}};
            rightNodeSecond->p = argv.v[i]->second;
            util_link(st_baseTail, rightNodeSecond);

            _node<pair<FirstType, SecondType>> *ll_node {new _node<pair<FirstType, SecondType>> {}};
            ll_node->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
            util_link_pair(linkedlist.tail, ll_node);
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
            util_link_pair(linkedlist.tail, ll_node);
        }

        size = argv.n > n ? argv.n : n;
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
        while (ll_dnode != nullptr && ll_dnode != linkedlist.tail) {
            delete ll_dnode->p;
            _node<pair<FirstType, SecondType>> *tnode {ll_dnode};
            ll_dnode = ll_dnode->right;
            delete tnode;
        }
        delete linkedlist.tail;
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
