#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#define TEMPLATE_CLASS \
template<\
    typename FirstType, \
    typename SecondType = int, \
    typename Hash = ull (*) (const FirstType &), \
    typename Compare = bool (*) (const FirstType &lhs, const FirstType &rhs), \
    int N = -1\
>

#define TEMPLATE_FUNC \
template<\
    typename FirstType, \
    typename SecondType, \
    typename Hash, \
    typename Compare, \
    int N\
>

namespace ds {

    typedef unsigned long long ull;

    template<typename Type>
    inline ull default_hash(const Type &);
    template<typename Type>
    inline ull default_hash<int>(int);

    template<typename Type>
    inline bool default_compare(const Type &, const Type &);
    template<typename Type>
    inline ull default_hash<int, int>(int, int);

    TEMPLATE_CLASS class structure {

        private:
        
        public:
        Hash hash;
        Compare compare;

        structure(const Hash &hash = default_hash, const Compare &compare = default_compare);
    };

    TEMPLATE_FUNC structure<FirstType, SecondType, Hash, Compare, N>::structure(const Hash &hash, const Compare &compare) : 
        hash {hash}, compare {compare} 
    {}

    template<typename Type>
    inline ull default_hash(const Type &value) {
        return value.hash();
    }
    template<typename Type>
    inline ull default_hash<int>(int value) {
        return value;
    }

    template<typename Type>
    inline bool default_compare(const Type &lhs, const Type &rhs) {
        return lhs < rhs;
    }
    template<typename Type>
    inline ull default_hash<int, int>(int lhs, int rhs) {
        return lhs < rhs;
    }
}

#undef TEMPLATE_CLASS
#undef TEMPLATE_FUNC

#endif
