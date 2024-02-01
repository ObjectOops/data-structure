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

    ds::str s1 {"test1"};
    ds::str s2 {"test2"};
    const ds::str s3 {"test1"};
    const ds::str s4 {"test2"};
    ds::structure<ds::str> test_str {};
    const ds::structure<ds::str> test_str_const {};

    assert(ds::default_compare(s1, s2));
    assert(!(ds::default_compare(s2, s1)));
    assert(ds::default_compare(s3, s4));
    assert(!(ds::default_compare(s4, s3)));

    assert(test_str.compare(s1, s2));
    assert(!(test_str.compare(s2, s1)));
    assert(test_str.compare(s3, s4));
    assert(!(test_str.compare(s4, s3)));

    assert(test_str_const.compare(s1, s2));
    assert(!(test_str_const.compare(s2, s1)));
    assert(test_str_const.compare(s3, s4));
    assert(!(test_str_const.compare(s4, s3)));

    return 0;
}
