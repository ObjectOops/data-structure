#include <cassert>
#include "../include/data_structure.hpp"

int main() {

    for (int i {}; i < 0xF; ++i) {
        if (i % 2 == 0) {
            ds::ipair<int, double> arguments [] {{1, 2.1}, {3, 4.32423}, {5, 6.19}};
            ds::structure<int, double> test {0xFF, ds::args(arguments)};
            ds::structure<int, int> test2 {0xFF, ds::args<int>(1, 2, 3, 4, 5)};

            const ds::ipair<int, double> arguments_const [] {{1, 2.1}, {3, 4.32423}, {5, 6.19}};
            const ds::structure<int, double> test_const {0xFF, ds::args(arguments_const)};
            const ds::structure<int, int> test2_const {0xFF, ds::args<int>(1, 2, 3, 4, 5)};
        }
    }

    return 0;
}
