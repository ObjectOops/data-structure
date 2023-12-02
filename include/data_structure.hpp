#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include <string.h>
#include <malloc.h>

// Specifies to use a function pointer to the hash function for type `T`.
#define dhash_t(T) ds::ull (*) (const T &)
// Specifies to use a function pointer to the compare function for type `T`.
#define dcomp_t(T) bool (*) (const T &, const T &)

#define CLASS_TEMPLATE \
template<\
    typename FirstType, \
    typename SecondType = int, \
    typename Hash = dhash_t(FirstType), \
    typename Compare = dcomp_t(FirstType), \
    bool stInit = false \
>

#define FUNC_TEMPLATE \
template<\
    typename FirstType, \
    typename SecondType, \
    typename Hash, \
    typename Compare, \
    bool stInit \
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
        template<typename Type3>
        friend class iterator;

        private:
        Type *p {nullptr};
        _node<Type> *left {nullptr}, *right {nullptr};

        _node() {
        }
    };

    // Pair type.
    // Important: The second value may not be initialized. Check availability with `secondExists()`.
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

        Type1 &first() noexcept {
            return *n1->p;
        }
        const Type1 &first() const noexcept {
            return const_cast<pair<Type1, Type2> *>(this)->first();
        }
        Type2 &second() {
            if (n2->p == nullptr) {
                throw exception::null_access {
                    "Second value in pair is not initialized. this->second() is invalid."
                };
            }
            return *n2->p;
        }
        const Type2 &second() const {
            return const_cast<pair<Type1, Type2> *>(this)->second();
        }

        bool secondExists() const noexcept {
            return n2->p != nullptr;
        }
    };

    template<typename Type>
    class iterator; // Class prototype;
    
    // The Data Structure.
    CLASS_TEMPLATE class structure {

        private:
        _node<FirstType> *ft_baseHead {nullptr}, *ft_baseTail {nullptr};
        _node<SecondType> *st_baseHead {nullptr}, *st_baseTail {nullptr};
        _node<pair<FirstType, SecondType>> *pair_baseHead {nullptr}, *pair_baseTail {nullptr};
        ull size {};

        struct _cmd {
            enum TYPE {INSERT, REMOVE};
            TYPE type;
            ull data;
        };
        _node<_cmd> *cmd_head {nullptr}, *cmd_tail {nullptr};

        struct secondary {

            protected:
            structure<FirstType, SecondType, Hash, Compare, stInit> *primary;
        };

        template<typename _LLT>
        struct linkedlist_secondary : public secondary {

            friend class structure;
            
            private:
            _node<_LLT> *head {nullptr};
            _node<_LLT> *tail {new _node<_LLT> {}};

            linkedlist_secondary(structure<FirstType, SecondType, Hash, Compare, stInit> *p) {
                secondary::primary = p;
            }

            void refresh(_node<_LLT> *node) {
                tail->left = node;
                node->right = tail;
            }

            public:
            iterator<pair<FirstType, SecondType>> begin() const {
                return iterator<pair<FirstType, SecondType>> {secondary::primary, head};
            }

            iterator<pair<FirstType, SecondType>> end() const {
                return iterator<pair<FirstType, SecondType>> {secondary::primary, tail};
            }
        };

        template<typename _nodeType>
        void util_link(_nodeType *&tail, _nodeType *&newNode) { // `tail` must be by reference.
            newNode->left = tail;
            tail->right = newNode;
            tail = newNode;
        }
        SecondType *util_initSecondType() {
            return stInit ? new SecondType {} : nullptr;
        }
        void util_initStageOne(FirstType *v1, SecondType *v2) {
            ft_baseHead = new _node<FirstType> {};
            ft_baseHead->p = v1;
            ft_baseTail = ft_baseHead;

            st_baseHead = new _node<SecondType> {};
            st_baseHead->p = v2;
            st_baseTail = st_baseHead;

            pair_baseHead = new _node<pair<FirstType, SecondType>> {};
            pair_baseHead->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
            pair_baseTail = pair_baseHead;

            pairll.head = pair_baseHead;
            pairll.refresh(pair_baseTail);
        }
        void util_initStageTwo(FirstType *v1, SecondType *v2) {
            _node<FirstType> *rightNode {new _node<FirstType> {}};
            rightNode->p = v1;
            util_link(ft_baseTail, rightNode);

            _node<SecondType> *rightNodeSecond {new _node<SecondType> {}};
            rightNodeSecond->p = v2;
            util_link(st_baseTail, rightNodeSecond);

            _node<pair<FirstType, SecondType>> *pairRightNode {new _node<pair<FirstType, SecondType>> {}};
            pairRightNode->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
            util_link(pair_baseTail, pairRightNode);

            pairll.refresh(pair_baseTail);
        }
        template<typename _nodeType>
        void util_deallocInternal(_nodeType *head, void *stop) {
            _nodeType *dnode {head};
            while (dnode != stop) {
                delete dnode->p;
                _nodeType *tnode {dnode};
                dnode = dnode->right;
                delete tnode;
            }
        }

        public:
        Hash hash;
        Compare compare;

        linkedlist_secondary<FirstType> ftll {this};
        linkedlist_secondary<SecondType> stll {this};
        linkedlist_secondary<pair<FirstType, SecondType>> pairll {this};

        structure(_argv<FirstType> = args<FirstType>(), ull = 0, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(_argv<ipair<FirstType, SecondType>>, ull = 0, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(ull, const Hash &hash = default_hash, const Compare &compare = default_compare);
        structure(const structure<FirstType, SecondType, Hash, Compare, stInit> &other);
        structure(const structure<FirstType, SecondType, Hash, Compare, stInit> &&other) noexcept;
        ~structure();
    };

    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare, stInit>::structure(
        _argv<FirstType> argv, 
        ull n, 
        const Hash &hash, 
        const Compare &compare
    ) : 
        hash {hash}, compare {compare} 
    {
        ull i {};
        if (argv.n > 0) {
            util_initStageOne(new FirstType {*argv.v[0]}, util_initSecondType());
            ++i;
        }
        for (; i < argv.n; ++i) {
            util_initStageTwo(new FirstType {*argv.v[i]}, util_initSecondType());
        }
        for (; i < n; ++i) {
            util_initStageTwo(new FirstType {}, util_initSecondType());
        }

        size = argv.n > n ? argv.n : n;
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare, stInit>::structure(
        _argv<ipair<FirstType, SecondType>> argv, 
        ull n, 
        const Hash &hash, 
        const Compare &compare
    ) : 
        hash {hash}, compare {compare} 
    {
        ull i {};
        if (argv.n > 0) {
            util_initStageOne(argv.v[0]->first, argv.v[0]->second);
            ++i;
        }
        for (; i < argv.n; ++i) {
            util_initStageTwo(argv.v[i]->first, argv.v[i]->second);
        }
        for (; i < n; ++i) {
            util_initStageTwo(new FirstType {}, util_initSecondType());
        }

        size = argv.n > n ? argv.n : n;
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare, stInit>::structure(
        ull n, 
        const Hash &hash, 
        const Compare &compare
    ) {
        structure(args<FirstType>(), n, hash, compare);
    }
    FUNC_TEMPLATE structure<FirstType, SecondType, Hash, Compare, stInit>::~structure() {
        util_deallocInternal(ft_baseHead, nullptr);
        util_deallocInternal(st_baseHead, nullptr);

        if (pair_baseHead != nullptr) {
            util_deallocInternal(pair_baseHead, pairll.tail);
        }

        delete ftll.tail;
        delete stll.tail;
        delete pairll.tail;
    }

    // Iterator over an instance of structure.
    // `Type` can be equivalent to one of the following: `FirstType`, `SecondType`, or `pair<FirstType, SecondType`.
    template<typename Type>
    class iterator {

        FUNC_TEMPLATE
        friend class structure;

        private:
        void *primary;
        _node<Type> *p;

        iterator(void *primary, _node<Type> *p) : primary {primary}, p {p} {
        }

        void test_node_ptr() const {
            if (p == nullptr) {
                throw exception::null_access {"Iterator does not reference a valid starting point."};
            }
        }
        void test_value_ptr() const {
            if (p->p == nullptr) {
                throw exception::null_access {"Iterator references null value."};
            }
        }
        void test_ptr_2() const {
            test_node_ptr();
            test_value_ptr();
        }

        public:
        iterator() : iterator {nullptr, nullptr} {
        }
        iterator &operator=(const iterator &other) noexcept {
            this->primary = other.primary;
            this->p = other.p;
            return *this;
        }
        bool operator==(const iterator &other) const noexcept {
            return this->p == other.p;
        }
        bool valid() const noexcept {
            return p != nullptr && p->p != nullptr;
        }

        Type &value() {
            test_ptr_2();
            return *p->p;
        }
        const Type &value() const {
            return const_cast<iterator<Type> *>(this)->value();
        }
        Type &operator*() {
            test_ptr_2();
            return *p->p;
        }
        const Type &operator*() const {
            return const_cast<iterator<Type> *>(this)->operator*();
        }
        Type *operator->() {
            return &this->operator*();
        }
        const Type *operator->() const {
            return const_cast<iterator<Type> *>(this)->operator->();
        }

        bool hasNext() const {
            test_node_ptr();
            return p->right != nullptr;
        }
        bool hasPrev() const {
            test_node_ptr();
            return p->left != nullptr;
        }

        Type &next() {
            test_ptr_2();
            Type &ret {*p->p};
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
            test_node_ptr();
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
            test_node_ptr();
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

        Type &prev() {
            test_ptr_2();
            Type &ret {*p->p};
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
            test_node_ptr();
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
            test_node_ptr();
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
