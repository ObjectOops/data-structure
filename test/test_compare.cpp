#include <cassert>
#include "../include/data_structure.hpp"

int main() {
    ds::structure<int> test {};

    assert(ds::default_compare(0, 1) == (0 < 1));
    assert(ds::default_compare(1, 0) == (1 < 0));
    assert(ds::default_compare(1, 1) == (1 < 1));

    assert(test.compare(0, 1) == (0 < 1));
    assert(test.compare(1, 0) == (1 < 0));
    assert(test.compare(1, 1) == (1 < 1));

    return 0;
}
