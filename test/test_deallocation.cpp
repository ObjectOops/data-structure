#include <cassert>
#include "../include/data_structure.hpp"

int main() {

    for (int i {}; i < 0xF; ++i) {
        if (i % 2 == 0) {
            // ds::ipair<int, double> arguments [] {{1, 2.1}, {3, 4.32423}, {5, 6.19}};
            // ds::structure<int, double> test {ds::args(arguments), 0xFF};

            ds::structure<int, int> test2 {ds::args<int>(1, 2, 3, 4, 5), 0xFF};
        }
    }

    return 0;
}
