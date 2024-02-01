#include "../../include/data_structure.hpp"

int main() {

    ds::structure<ds::ull> test {ds::args<int>(1, 2, 3)};
    const ds::structure<ds::ull> test_const {ds::args<int>(1, 2, 3)};

    return 0;
}
