#include "../../include/data_structure.hpp"

struct obj {
    int a;
    bool b;
};

struct functor1 {
    inline ds::ull operator()(const obj &lhs, const obj &rhs) {
        if (lhs.a == rhs.a) {
            return lhs.b == rhs.b;
        }
        return lhs.a < rhs.a;
    }
};

struct functor2 {
    inline ds::ull operator()(const obj &o) {
        return o.a + 0xFFFF * o.b;
    }
};

int main() {

    ds::structure<obj, int, functor1, functor2> test {64, functor1 {}, functor2 {}};

    return 0;
}
