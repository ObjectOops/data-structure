#include <cassert>

#include "../include/data_structure.hpp"

int main() {
    ds::structure<int> test {};
    
    assert(ds::default_hash(0) == 0ull);
    assert(ds::default_hash(0xFFFF) == 0xFFFFull);
    assert(ds::default_hash(-0xFFFF) == 0xFFFFFFFFFFFF0001);
    
    assert(test.hash(0) == 0ull);
    assert(test.hash(0xFFFF) == 0xFFFFull);
    assert(test.hash(-0xFFFF) == 0xFFFFFFFFFFFF0001);
    
    return 0;
}
