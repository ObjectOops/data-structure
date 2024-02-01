#include "../../include/data_structure.hpp"

int main() {

    ds::ipair<int, double> arguments [] {{1, 2.1}, {3, 4.32423}, {5, 6.19}};
    ds::structure<int, double> test {ds::args(arguments)};

    const ds::ipair<int, double> arguments_const [] {{1, 2.1}, {3, 4.32423}, {5, 6.19}};
    const ds::structure<int, double> test_const {ds::args(arguments)};

    return 0;
}
