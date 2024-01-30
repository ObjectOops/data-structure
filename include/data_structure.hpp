#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include <malloc.h>
#include <string.h>

// Specifies to use a function pointer to the hash function for type `T`.
#define dhash_t(T) ds::ull (*) (const T &)
// Specifies to use a function pointer to the compare function for type `T`.
#define dcomp_t(T) bool (*) (const T &, const T &)

// `if constexpr` when -std=c++17 or higher, `if` otherwise.
#if __cplusplus >= 201703L
#define if_constexpr if constexpr
#else
#define if_constexpr if
#endif

namespace ds {

    typedef unsigned long long ull;

    // Begin Preceding Definitions ====================================

    // Throwing exceptions from `ds::exception` are for internal use only!
    namespace exception {

        // Base exception.
        struct base {

            protected:
            char *msg;

            public:
            base(
                const char * = "ds::base unnamed exception", 
                const char * = "An exception was thrown.", 
                bool = false, 
                const char * = ":\n\t"
            );
            base(const base &);
            base(base &&) noexcept;
            ~base();

            const char *what() const noexcept;
        };
        base::base(
            const char *exceptionName, 
            const char *content, 
            bool selfDestruct, 
            const char *separator
        ) {
            ull size {strlen(exceptionName) + strlen(separator) + strlen(content) + 1};
            this->msg = new char [size];
            snprintf(this->msg, size, "%s%s%s", exceptionName, separator, content);
            
            // Delete `content` if allocated with new. May change later.
            if (selfDestruct) {
                delete[] content;
            }
        }
        base::base(const base &other) {
            ull size {strlen(other.msg) + 1};
            this->msg = new char [size];
            memcpy(msg, other.msg, size);
            this->msg[size - 1] = '\0';
        }
        base::base(base &&other) noexcept {
            this->msg = other.msg;
            other.msg = nullptr;
        }
        base::~base() {
            delete[] msg;
        }

        const char *base::what() const noexcept {
            return msg;
        }

        // Out-of-bounds exception.
        struct out_of_bounds : public base {

            public:
            out_of_bounds(const char *, bool = false);
        };
        out_of_bounds::out_of_bounds(const char *msg, bool selfDestruct) : 
            base {"ds::out_of_bounds exception", msg, selfDestruct} 
        {
        }

        // Null / invalid data access exception.
        struct null_access : public base {

            public:
            null_access(const char *);
        };
        null_access::null_access(const char *msg) : 
            base {"ds::null_access exception", msg, false} 
        {
        }
    }

    class str;
    namespace literals {
        str operator "" _s(const char *, size_t);
    }

    // Basic string class.
    class str {

        friend str literals::operator "" _s(const char *, size_t);

        private:
        char *s;
        ull len;

        // Constructor for internal use.
        str(char *, ull);

        public:        
        str(const char *);
        str(const str &);
        str(str &&) noexcept;
        str();
        ~str();
        ull length() const noexcept;
        str substr(ull, ull) const;
        const char *cstr() const noexcept;
        char &operator[](ull);
        const char &operator[](ull) const;
        str &operator=(const str &);
        str &operator=(str &&);
        str operator+(const str &) const;
        str &operator+=(const str &);
        bool operator==(const str &) const noexcept;
    };
    // Constructor for internal use only!
    str::str(char *_s, ull _len) : s {_s}, len {_len} {
    }
    str::str(const char *src) {
        this->len = strlen(src);
        this->s = new char [this->len + 1];
        memcpy(this->s, src, len);
        this->s[len] = '\0';
    }
    str::str(const str &obj) : str {obj.s} {
    }
    str::str(str &&obj) noexcept {
        this->s = obj.s;
        this->len = obj.len;
        obj.s = nullptr;
        obj.len = 0;
    }
    str::str() : str {""} {
    }
    str::~str() {
        delete[] s;
    }
    ull str::length() const noexcept {
        return len;
    }
    str str::substr(ull start, ull end) const {
        start = start < len ? start : len;
        end = end < len ? end : len;
        ull len = end - start;

        char *buffer {new char [len + 1]};
        memcpy(buffer, s + start, len);
        buffer[len] = '\0';

        return str {buffer, len};
    }
    const char *str::cstr() const noexcept {
        return s;
    }
    char &str::operator[](ull index) {
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
    const char &str::operator[](ull index) const {
        return const_cast<str *>(this)->operator[](index);
    }
    str &str::operator=(const str &other) {
        if (this == &other) {
            return *this;
        }
        delete[] s;
        s = new char [other.len + 1];
        memcpy(s, other.s, other.len);
        s[other.len] = '\0';
        len = other.len;
        return *this;
    }
    str &str::operator=(str &&other) {
        if (this == &other) {
            return *this;
        }
        delete[] this->s;
        this->s = other.s;
        this->len = other.len;
        other.s = nullptr;
        other.len = 0;
        return *this;
    }
    str str::operator+(const str &other) const {
        ull size = len + other.len + 1;
        char *buffer {new char [size]};

        memcpy(buffer, s, len);
        memcpy(buffer + len, other.s, other.len);
        buffer[size - 1] = '\0';

        return str {buffer, size - 1};
    }
    str &str::operator+=(const str &other) {
        return operator=(operator+(other));
    }
    bool str::operator==(const str &other) const noexcept {
        return strncmp(s, other.s, len < other.len ? other.len : len) == 0;
    }

    namespace literals {
        // `ds::str` custom literal.
        str operator "" _s(const char *raw, size_t size) {
            char *s = new char [size];
            memcpy(s, raw, size);
            return str {s, size - 1};
        }
    }

    // End Preceding Definitions ======================================

    // Begin Forward Declarations =====================================

    template<typename T1, typename T2>
    struct ipair;

    template<typename T>
    struct _argv;

    template<typename Type, typename ...Types>
    _argv<Type> args(const Types &...);

    template<typename Type>
    struct _node;

    template<typename Type1, typename Type2>
    class pair;

    template<typename Type>
    class iterator;

    // The class templates require the default hash and comparison functions 
    // to take all parameters by reference, including primitive types.

    template<typename Type>
    ull default_hash(const Type &);

    template<typename Type>
    bool default_compare(const Type &, const Type &);

    // End Forward Declarations =======================================

#define CLASS_TEMPLATE \
template<\
    typename FirstType, \
    typename SecondType = int, \
    typename Compare = dcomp_t(FirstType), \
    typename Hash = dhash_t(FirstType), \
    bool stInit = false \
>

#define FUNC_TEMPLATE \
template<\
    typename FirstType, \
    typename SecondType, \
    typename Compare, \
    typename Hash, \
    bool stInit \
>

#define TYPE_CLASS structure<FirstType, SecondType, Compare, Hash, stInit>

#define STRUCTURE(RET) FUNC_TEMPLATE RET TYPE_CLASS

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
            TYPE_CLASS *primary;
        };

#define LL(RET) FUNC_TEMPLATE template<typename _LLT> RET TYPE_CLASS::linkedlist_secondary<_LLT>

        template<typename _LLT>
        struct linkedlist_secondary : public secondary {

            friend class structure;
            
            private:
            _node<_LLT> *head {nullptr};
            _node<_LLT> *tail {new _node<_LLT> {}};

            linkedlist_secondary(TYPE_CLASS *);

            void refresh(_node<_LLT> *);

            public:
            iterator<_LLT> begin() const;
            iterator<_LLT> end() const;
        };

#define LL_EX(RET) FUNC_TEMPLATE template<typename _LLT> RET TYPE_CLASS::linkedlist_secondary_ex<_LLT>

        template<typename _LLT>
        struct linkedlist_secondary_ex : public linkedlist_secondary<_LLT> {
            linkedlist_secondary_ex(TYPE_CLASS *);
        };

        template<typename _nodeType>
        void util_link(_nodeType *&, _nodeType *&);
        SecondType *util_initSecondType();
        void util_initStageOne(FirstType *, SecondType *);
        void util_initStageTwo(FirstType *, SecondType *);
        template<typename _nodeType>
        void util_deallocInternal(_nodeType *, void *);

        public:
        Hash hash;
        Compare compare;

        linkedlist_secondary_ex<FirstType> ftll {this};
        linkedlist_secondary<SecondType> stll {this};
        linkedlist_secondary_ex<pair<FirstType, SecondType>> pairll {this};

        // Quantity and elements.
        structure(ull, _argv<FirstType>, const Compare & = default_compare, const Hash & = default_hash);
        // Elements only.
        structure(_argv<FirstType>, const Compare & = default_compare, const Hash & = default_hash);
        // Quantity only.
        structure(ull, const Compare & = default_compare, const Hash & = default_hash);
        // Neither quantity nor elements.
        structure(const Compare & = default_compare, const Hash & = default_hash);
        // Quantity and map.
        structure(ull, _argv<ipair<FirstType, SecondType>>, const Compare & = default_compare, const Hash & = default_hash);
        // Map only.
        structure(_argv<ipair<FirstType, SecondType>>, const Compare & = default_compare, const Hash & = default_hash);

        structure(const TYPE_CLASS &);
        structure(const TYPE_CLASS &&) noexcept;
        ~structure();
    };

    // Begin Data Structure Method Definitions ========================

    // Private

    STRUCTURE()::structure(
        ull n, 
        _argv<FirstType> argv, 
        const Compare &compare, 
        const Hash &hash
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
    STRUCTURE()::structure(
        _argv<FirstType> argv, 
        const Compare &compare, 
        const Hash &hash
    ) : structure {0, argv, compare, hash} 
    {
        // Prevent double free that can occur due to use of implicit copy constructor.
        argv.v = nullptr;
    }
    STRUCTURE()::structure(
        ull n, 
        const Compare &compare, 
        const Hash &hash
    ) : structure {n, args<FirstType>(), compare, hash} 
    {
    }
    STRUCTURE()::structure(
        const Compare &compare, 
        const Hash &hash
    ) : structure {0, args<FirstType>(), compare, hash} 
    {
    }
    STRUCTURE()::structure(
        ull n, 
        _argv<ipair<FirstType, SecondType>> argv, 
        const Compare &compare, 
        const Hash &hash
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
    STRUCTURE()::structure(
        _argv<ipair<FirstType, SecondType>> argv, 
        const Compare &compare, 
        const Hash &hash
    ) : structure {0, argv, compare, hash} 
    {
        // Prevent double free that can occur due to use of implicit copy constructor.
        argv.v = nullptr;
    }
    STRUCTURE()::~structure() {
        if (ft_baseHead || st_baseHead || pair_baseHead) {
            util_deallocInternal(ft_baseHead, ftll.tail);
            util_deallocInternal(st_baseHead, stll.tail);
            util_deallocInternal(pair_baseHead, pairll.tail);
        }

        delete ftll.tail;
        delete stll.tail;
        delete pairll.tail;
    }

    STRUCTURE(template<typename _nodeType> void)::util_link(
        _nodeType *&tail, _nodeType *&newNode // `tail` must be by reference.
    ) {
        newNode->left = tail;
        tail->right = newNode;
        tail = newNode;
    }
    STRUCTURE(SecondType *)::util_initSecondType() {
        return stInit ? new SecondType {} : nullptr;
    }
    STRUCTURE(void)::util_initStageOne(FirstType *v1, SecondType *v2) {
        ft_baseHead = new _node<FirstType> {};
        ft_baseHead->p = v1;
        ft_baseTail = ft_baseHead;

        st_baseHead = new _node<SecondType> {};
        st_baseHead->p = v2;
        st_baseTail = st_baseHead;

        pair_baseHead = new _node<pair<FirstType, SecondType>> {};
        pair_baseHead->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
        pair_baseTail = pair_baseHead;

        ftll.head = ft_baseHead;
        stll.head = st_baseHead;
        pairll.head = pair_baseHead;
        ftll.refresh(ft_baseTail);
        stll.refresh(st_baseTail);
        pairll.refresh(pair_baseTail);
    }
    STRUCTURE(void)::util_initStageTwo(FirstType *v1, SecondType *v2) {
        _node<FirstType> *rightNode {new _node<FirstType> {}};
        rightNode->p = v1;
        util_link(ft_baseTail, rightNode);

        _node<SecondType> *rightNodeSecond {new _node<SecondType> {}};
        rightNodeSecond->p = v2;
        util_link(st_baseTail, rightNodeSecond);

        _node<pair<FirstType, SecondType>> *pairRightNode {new _node<pair<FirstType, SecondType>> {}};
        pairRightNode->p = new pair<FirstType, SecondType> {ft_baseTail, st_baseTail};
        util_link(pair_baseTail, pairRightNode);

        ftll.refresh(ft_baseTail);
        stll.refresh(st_baseTail);
        pairll.refresh(pair_baseTail);
    }
    STRUCTURE(template<typename _nodeType> void)::util_deallocInternal(
        _nodeType *head, void *stop
    ) {
        _nodeType *dnode {head};
        while (dnode != stop) {
            delete dnode->p;
            _nodeType *tnode {dnode};
            dnode = dnode->right;
            delete tnode;
        }
    }

    // Public

    // End Data Structure Method Definitions ==========================

    // Begin Linked List Method Definitions ===========================

    // Private

    LL()::linkedlist_secondary(TYPE_CLASS *p) {
        secondary::primary = p;
    }
    LL_EX()::linkedlist_secondary_ex(TYPE_CLASS *p) : linkedlist_secondary<_LLT> {p} {
    }
    LL(void)::refresh(_node<_LLT> *node) {
        tail->left = node;
        node->right = tail;
    }

    // Public

    LL(iterator<_LLT>)::begin() const {
        return iterator<_LLT> {secondary::primary, head};
    }
    LL(iterator<_LLT>)::end() const {
        return iterator<_LLT> {secondary::primary, tail};
    }

    // End Linked List Method Definitions =============================

    // Initialization pair. Make an array of these to initialize values to pass to `args`.
    template<typename T1, typename T2>
    struct ipair {

        FUNC_TEMPLATE
        friend class structure;

        private:
        T1 *first;
        T2 *second;

        public:
        ipair(const T1 &, const T2 &);
    };
    template<typename T1, typename T2>
    ipair<T1, T2>::ipair(const T1 &first, const T2 &second) : 
        first {new T1 {first}}, second {new T2 {second}} 
    {
    }

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
        _argv(const T **, ull);

        public:
        ~_argv();
    };
    template<typename T>
    _argv<T>::_argv(const T **v, ull n) : v {v}, n {n} {
    }
    template<typename T>
    _argv<T>::~_argv() {
        delete[] v;
    }

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

        _node();
    };
    template<typename Type>
    _node<Type>::_node() {
    }

#define PAIR(RET) template<typename Type1, typename Type2> RET pair<Type1, Type2>

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

        pair(_node<Type1> *, _node<Type2> *);

        public:
        pair(const Type1 &, const Type2 &);
        pair(const Type1 &);
        pair();
        pair(const pair<Type1, Type2> &);
        pair(const pair<Type1, Type2> &&);
        ~pair();
        Type1 &first() noexcept;
        const Type1 &first() const noexcept;
        Type2 &second();
        const Type2 &second() const;
        bool secondExists() const noexcept;
    };
    PAIR()::pair(_node<Type1> *n1, _node<Type2> *n2) : n1 {n1}, n2 {n2}, ref {true} {
    }
    PAIR()::pair(const Type1 &first, const Type2 &second) : ref {false} {
        n1 = new _node<Type1> {};
        n2 = new _node<Type2> {};
        n1->p = new Type1 {first};
        n2->p = new Type2 {second};
    }
    PAIR()::pair(const Type1 &first) : ref {false} {
        n1 = new _node<Type1> {};
        n2 = new _node<Type2> {};
        n1->p = new Type1 {first};
        n2->p = nullptr;
    }
    PAIR()::pair() : pair(Type1 {}, Type2 {}) {
    }
    PAIR()::pair(const pair<Type1, Type2> &other) : pair {*other.n1->p, *other.n2->p} {
    }
    PAIR()::pair(const pair<Type1, Type2> &&other) {
        this->n1 = other.n1;
        this->n2 = other.n2;
        other.n1 = nullptr;
        other.n2 = nullptr;
    }
    PAIR()::~pair() {
        if (!ref) {
            delete n1->p;
            delete n2->p;
            delete n1;
            delete n2;
        }
    }
    PAIR(Type1 &)::first() noexcept {
        return *n1->p;
    }
    PAIR(const Type1 &)::first() const noexcept {
        return const_cast<pair<Type1, Type2> *>(this)->first();
    }
    PAIR(Type2 &)::second() {
        if (n2->p == nullptr) {
            throw exception::null_access {
                "Second value in pair is not initialized. this->second() is invalid."
            };
        }
        return *n2->p;
    }
    PAIR(const Type2 &)::second() const {
        return const_cast<pair<Type1, Type2> *>(this)->second();
    }
    PAIR(bool)::secondExists() const noexcept {
        return n2->p != nullptr;
    }

#define ITER(RET) template<typename Type> RET iterator<Type>

    // Iterator over an instance of structure.
    // `Type` can be equivalent to one of the following: `FirstType`, `SecondType`, or `pair<FirstType, SecondType`.
    template<typename Type>
    class iterator {

        FUNC_TEMPLATE
        friend class structure;

        private:
        void *primary;
        _node<Type> *p;

        iterator(void *, _node<Type> *);

        void test_node_ptr() const;
        void test_value_ptr() const;
        void test_ptr_2() const;

        public:
        iterator();
        iterator &operator=(const iterator &) noexcept;
        bool operator==(const iterator &) const noexcept;
        bool valid() const noexcept;
        Type &value();
        const Type &value() const;
        Type &operator*();
        const Type &operator*() const;
        Type *operator->();
        const Type *operator->() const;
        bool hasNext() const;
        bool hasPrev() const;
        Type &next();
        iterator &operator++();
        iterator &operator++(int);
        Type &prev();
        iterator &operator--();
        iterator &operator--(int);
    };
    ITER()::iterator(void *primary, _node<Type> *p) : primary {primary}, p {p} {
    }
    ITER(void)::test_node_ptr() const {
        if (p == nullptr) {
            throw exception::null_access {"Iterator does not reference a valid starting point."};
        }
    }
    ITER(void)::test_value_ptr() const {
        if (p->p == nullptr) {
            throw exception::null_access {"Iterator references null value."};
        }
    }
    ITER(void)::test_ptr_2() const {
        test_node_ptr();
        test_value_ptr();
    }
    ITER()::iterator() : iterator {nullptr, nullptr} {
    }
    ITER(iterator<Type> &)::operator=(const iterator &other) noexcept {
        this->primary = other.primary;
        this->p = other.p;
        return *this;
    }
    ITER(bool)::operator==(const iterator &other) const noexcept {
        return this->p == other.p;
    }
    ITER(bool)::valid() const noexcept {
        return p != nullptr && p->p != nullptr;
    }
    ITER(Type &)::value() {
        test_ptr_2();
        return *p->p;
    }
    ITER(const Type &)::value() const {
        return const_cast<iterator<Type> *>(this)->value();
    }
    ITER(Type &)::operator*() {
        test_ptr_2();
        return *p->p;
    }
    ITER(const Type &)::operator*() const {
        return const_cast<iterator<Type> *>(this)->operator*();
    }
    ITER(Type *)::operator->() {
        return &this->operator*();
    }
    ITER(const Type *)::operator->() const {
        return const_cast<iterator<Type> *>(this)->operator->();
    }
    ITER(bool)::hasNext() const {
        test_node_ptr();
        return p->right != nullptr;
    }
    ITER(bool)::hasPrev() const {
        test_node_ptr();
        return p->left != nullptr;
    }
    ITER(Type &)::next() {
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
    ITER(iterator<Type> &)::operator++() { // Pre-increment.
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
    ITER(iterator<Type> &)::operator++(int) { // Post-increment.
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
    ITER(Type &)::prev() {
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
    ITER(iterator<Type> &)::operator--() { // Pre-decrement.
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
    ITER(iterator<Type> &)::operator--(int) { // Post-decrement.
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
}

#undef CLASS_TEMPLATE
#undef FUNC_TEMPLATE

#undef TYPE_CLASS
#undef STRUCTURE
#undef LL
#undef PAIR
#undef ITER

#undef DEFAULT_HASH
#undef DEFAULT_COMPARE

#endif
