#include <cassert>
#include <cstring>

#include "../include/data_structure.hpp"

int main() {

    ds::pair<int, int> test1;
    assert(test1.getFirst() == 0 && test1.getSecond() == 0);

    ds::pair<ds::str, int> test2 {ds::str {"test"}, 32};
    assert(test2.getFirst() == ds::str {"test"} && test2.getSecond() == 32);

    ds::pair<int, ds::str> test3;

    try {
        ++test3;
        assert(false && "Increment should not succeed.");
    } catch (ds::exception::out_of_bounds &e) {
        assert(strcmp(e.what(), "ds out-of-bounds exception:\n\tds::pair iterator pre-increment out of bounds, this->right is nullptr.") == 0);
    }
    try {
        test3++;
        assert(false && "Increment should not succeed.");
    } catch (ds::exception::out_of_bounds &e) {
        assert(strcmp(e.what(), "ds out-of-bounds exception:\n\tds::pair iterator post-increment out of bounds, this->right is nullptr.") == 0);
    }
    try {
        --test3;
        assert(false && "Decrement should not succeed.");
    } catch (ds::exception::out_of_bounds &e) {
        assert(strcmp(e.what(), "ds out-of-bounds exception:\n\tds::pair iterator pre-decrement out of bounds, this->left is nullptr.") == 0);
    }
    try {
        test3--;
        assert(false && "Decrement should not succeed.");
    } catch (ds::exception::out_of_bounds &e) {
        assert(strcmp(e.what(), "ds out-of-bounds exception:\n\tds::pair iterator post-decrement out of bounds, this->left is nullptr.") == 0);
    }

    return 0;
}
