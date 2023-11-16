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
    // assert(test3[-1] == -1);
    // assert(test3[test3.length()] == -1);
    try {
        test3[-1];
    } catch (ds::exception::out_of_bounds &e) {
        assert(strncmp(e.what(), "ds out-of-bounds exception:\n\tds::str string access at index", 59) == 0);
    }
    try {
        test3[test3.length()];
    } catch (ds::exception::out_of_bounds &e) {
        assert(strcmp(e.what(), "ds out-of-bounds exception:\n\tds::str string access at index 16 with length 16.") == 0);
    }
    test3[0] = 'w';
    assert(strcmp(test3.cstr(), "west again again") == 0);
    test3[0] = 't';

    assert(strcmp(test3.substr(0, 4).cstr(), "test") == 0);
    assert(strcmp(test3.substr(0, 64).cstr(), "test again again") == 0);
    assert(strcmp(test3.substr(64, 64).cstr(), "") == 0);

    assert(test3 == ds::str {"test again again"});
    assert(!(test3 == ds::str {"not equals"}));

    return 0;
}
