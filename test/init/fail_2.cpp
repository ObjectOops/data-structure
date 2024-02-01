#include "../../include/data_structure.hpp"

int main() {

    ds::structure<int> test {ds::args<ds::ull>(1, 2, 3)};
    const ds::structure<int> test_const {ds::args<ds::ull>(1, 2, 3)};

    return 0;
}
