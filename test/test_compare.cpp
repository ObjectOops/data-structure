#include <cassert>
#include <iostream>

#include "../include/data_structure.hpp"

#define TEST_TYPE(TYPE) \
ds::structure<TYPE> test_##TYPE {}; \
assert(ds::default_compare((TYPE)0, (TYPE)1) == ((TYPE)0 < (TYPE)1)); \
assert(ds::default_compare((TYPE)1, (TYPE)0) == ((TYPE)1 < (TYPE)0)); \
assert(ds::default_compare((TYPE)1, (TYPE)1) == ((TYPE)1 < (TYPE)1)); \
assert(test_##TYPE.compare((TYPE)0, (TYPE)1) == ((TYPE)0 < (TYPE)1)); \
assert(test_##TYPE.compare((TYPE)1, (TYPE)0) == ((TYPE)1 < (TYPE)0)); \
assert(test_##TYPE.compare((TYPE)1, (TYPE)1) == ((TYPE)1 < (TYPE)1));

typedef long long ll;
typedef unsigned short us;
typedef unsigned int ui;
typedef unsigned long ul;
typedef unsigned long long ull;

int main() {

    TEST_TYPE(short)
    TEST_TYPE(int)
    TEST_TYPE(long)
    TEST_TYPE(ll)
    TEST_TYPE(us)
    TEST_TYPE(ui)
    TEST_TYPE(ul)
    TEST_TYPE(ull)
    TEST_TYPE(char)
    TEST_TYPE(wchar_t)
    TEST_TYPE(char16_t)
    TEST_TYPE(char32_t)
    TEST_TYPE(bool)

    ds::str s1 {"test1", 5};
    ds::str s2 {"test2", 5};

    assert(ds::default_compare(s1, s2));
    assert(s1 < s2);
    assert(!(ds::default_compare(s2, s1)));
    assert(!(s2 < s1));

    return 0;
}
