#include "test_include_header.hpp"
#include "../include/data_structure.hpp"

int main() {

    ds::str test_str {"test"};
    ds::structure<int> test_structure {ds::args<int>(1, 2, 3, 4, 5)};
    ds::default_hash(0);
    ds::default_hash(test_str);
    ds::default_compare(0, 0);
    ds::default_compare(test_str, getStr());

    return 0;
}
