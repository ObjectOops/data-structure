#include <cassert>

#include "../include/data_structure.hpp"

int main() {
    ds::structure<int> test {};
    
    assert(ds::default_hash(0) == 0ull);
    assert(ds::default_hash(0xFFFF) == 0xFFFFull);
    // assert(ds::default_hash(-0xFFFF) == -0xFFFull);
    
    assert(test.test_hash(0) == 0ull);
    assert(test.test_hash(0xFFFF) == 0xFFFFull);
    // assert(test.test_hash(-0xFFFF) == -0xFFFull);
    
    return 0;
}
