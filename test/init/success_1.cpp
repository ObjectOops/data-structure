#include "../../include/data_structure.hpp"

int main() {

    ds::structure<int> test {ds::args<int>(1, 2, 3)};
    const ds::structure<int> test_const {ds::args<int>(1, 2, 3)};

    return 0;
}
