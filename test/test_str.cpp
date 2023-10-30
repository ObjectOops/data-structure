#include <cassert>

#include "../include/data_structure.hpp"

int main() {

    ds::str test {};
    assert(strcmp(test.cstr(), "") == 0);
    assert(test.length() == 0);

    ds::str test2 {"test"};
    assert(strcmp(test2.cstr(), "test") == 0);
    assert(test2.length() == 4);

    test = test2;
    assert(strcmp(test.cstr(), "test") == 0);
    assert(test.length() == 4);

    ds::str test3 {"again"};
    test3 = test2 + ds::str {" "} + test3;
    assert(strcmp(test3.cstr(), "test again") == 0);
    assert(test3.length() == strlen("test again"));

    test3 += ds::str {" again"};
    assert(strcmp(test3.cstr(), "test again again") == 0);
    assert(test3.length() == strlen("test again again"));

    assert(test3[0] == 't');
    assert(test3[test3.length() - 1] == 'n');
    assert(test3[-1] == -1);
    assert(test3[test3.length()] == -1);

    assert(strcmp(test3.substr(0, 4).cstr(), "test") == 0);
    assert(strcmp(test3.substr(0, 64).cstr(), "test again again") == 0);
    assert(strcmp(test3.substr(64, 64).cstr(), "") == 0);

    return 0;
}
