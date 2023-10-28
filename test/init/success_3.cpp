#include "../../include/data_structure.hpp"

ds::ull hash_double(const double &value) {
    return *((ds::ull *)(&value)) >> 4;
}

int main() {

    ds::structure<double> test {ds::args<double>(1.0, 2.0, 3.0), 0, hash_double};

    return 0;
}
