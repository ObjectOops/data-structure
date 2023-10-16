#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

namespace ds {

    typedef unsigned long long ull;

    ull default_hash(int);
    bool default_compare(int, int);

    template<
        typename Type, 
        typename Hash = ull (*) (Type), 
        typename Compare = bool (*) (Type lhs, Type rhs)
    > class structure {

        private:
        Hash hash;
        Compare compare;

        public:
        structure(const Hash &hash = default_hash, const Compare &compare = default_compare) : 
            hash {hash}, compare {compare} 
        {}

        ull test_hash(Type value) {
            return hash(value);
        }
        bool test_compare(Type lhs, Type rhs) {
            return compare(lhs, rhs);
        }
    };

    ull default_hash(int value) {
        return value;
    }

    bool default_compare(int lhs, int rhs) {
        return lhs < rhs;
    }
}

#endif
