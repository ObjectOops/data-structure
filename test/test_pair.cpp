#include <cassert>
#include <cstring>

#include "../include/data_structure.hpp"

int main() {

    ds::pair<int, int> test1;
    assert(test1.getFirst() == 0 && test1.getSecond() == 0);

    ds::pair<ds::str, int> test2 {ds::str {"test"}, 32};
    assert(test2.getFirst() == ds::str {"test"} && test2.getSecond() == 32);

    ds::structure<int, int> s {ds::args<int>(1, 2, 3)};
    auto iter {s.linkedlist.begin()};
    assert(!iter.value().secondExists());
    try {
        iter.value().getSecond();
        assert(false && "iter.value().getSecond() should not succeed.");
    } catch (ds::exception::null_access &e) {
    }

    return 0;
}
