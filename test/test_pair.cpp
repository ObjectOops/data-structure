#include <cassert>
#include <cstring>

#include "../include/data_structure.hpp"

int main() {

    {
        ds::pair<int, int> test1;
        assert(test1.first() == 0 && test1.second() == 0);

        ds::pair<ds::str, int> test2 {ds::str {"test"}, 32};
        assert(test2.first() == ds::str {"test"} && test2.second() == 32);

        ds::structure<int, int> s {ds::args<int>(1, 2, 3)};
        auto iter {s.pairll.begin()};
        assert(!iter.value().secondExists());
        try {
            iter.value().second();
            assert(false && "iter.value().second() should not succeed.");
        } catch (ds::exception::null_access &e) {
        }
    }

    {
        const ds::pair<int, int> test1;
        assert(test1.first() == 0 && test1.second() == 0);

        const ds::pair<ds::str, int> test2 {ds::str {"test"}, 32};
        assert(test2.first() == ds::str {"test"} && test2.second() == 32);

        ds::structure<int, int> s {ds::args<int>(1, 2, 3)};
        auto iter {s.pairll.begin()};
        assert(!iter.value().secondExists());
        try {
            iter.value().second();
            assert(false && "iter.value().second() should not succeed.");
        } catch (ds::exception::null_access &e) {
        }
    }

    return 0;
}
