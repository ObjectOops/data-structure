#include <cassert>
#include <cstring>

#include "../include/data_structure.hpp"

int main() {

    ds::pair<int, int> test1;
    assert(test1.getFirst() == 0 && test1.getSecond() == 0);

    ds::pair<ds::str, int> test2 {ds::str {"test"}, 32};
    assert(test2.getFirst() == ds::str {"test"} && test2.getSecond() == 32);

    return 0;
}
