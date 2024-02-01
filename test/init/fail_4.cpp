#include "../../include/data_structure.hpp"

int main() {

    ds::ipair<int, double> arguments [] {{1, 2.54353}, {2, 4534534}, {3, 5.5}};
    ds::structure<int, char> test {ds::args<int, char>(arguments)};

    const ds::ipair<int, double> arguments_const [] {{1, 2.54353}, {2, 4534534}, {3, 5.5}};
    const ds::structure<int, char> test_const {ds::args<int, char>(arguments)};

    return 0;
}
