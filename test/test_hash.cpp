#include <cassert>
#include <string>

#include "../include/data_structure.hpp"

#define TEST_TYPE(TYPE, NEGATIVE_RESULT) \
ds::structure<TYPE> test_##TYPE {}; \
assert(ds::default_hash((TYPE)0) == 0ull); \
assert(ds::default_hash((TYPE)0xFF) == 0xFFull); \
assert(ds::default_hash((TYPE)-0xFF) == NEGATIVE_RESULT); \
assert(test_##TYPE.hash((TYPE)0) == 0ull); \
assert(test_##TYPE.hash((TYPE)0xFF) == 0xFFull); \
assert(test_##TYPE.hash((TYPE)-0xFF) == NEGATIVE_RESULT);

#define TEST_TYPE_2(TYPE, NEGATIVE_RESULT, SECOND) \
ds::structure<TYPE> test_##TYPE {}; \
assert(ds::default_hash((TYPE)0) == 0ull); \
assert(ds::default_hash((TYPE)0xFF) == SECOND); \
assert(ds::default_hash((TYPE)-0xFF) == NEGATIVE_RESULT); \
assert(test_##TYPE.hash((TYPE)0) == 0ull); \
assert(test_##TYPE.hash((TYPE)0xFF) == SECOND); \
assert(test_##TYPE.hash((TYPE)-0xFF) == NEGATIVE_RESULT);

typedef long long ll;
typedef unsigned short us;
typedef unsigned int ui;
typedef unsigned long ul;
typedef unsigned long long ull;

int main() {

    TEST_TYPE(short, 0xFFFFFFFFFFFFFF01ull)
    TEST_TYPE(int, 0xFFFFFFFFFFFFFF01ull)
    TEST_TYPE(long, 0xFFFFFFFFFFFFFF01ull)
    TEST_TYPE(ll, 0xFFFFFFFFFFFFFF01ull)
    TEST_TYPE(us, 0xFF01ull)
    TEST_TYPE(ui, 0xFFFFFF01ull)
    TEST_TYPE(ul, 0xFFFFFFFFFFFFFF01ull)
    TEST_TYPE(ull, 0xFFFFFFFFFFFFFF01ull)
    TEST_TYPE_2(char, 1ull, 0xFFFFFFFFFFFFFFFFull)
    TEST_TYPE(wchar_t, 0xFFFFFFFFFFFFFF01ull)
    TEST_TYPE(char16_t, 0xFF01ull)
    TEST_TYPE(char32_t, 0xFFFFFF01ull)
    TEST_TYPE_2(bool, 1ull, 1ull)

    ds::str s;
    s.p = new char [1000];
    sprintf(s.p, "test");
    s.n = 4;
    ds::structure<ds::str> test_str {};
    assert(ds::default_hash(s) == 0x74736574ull);

    std::string chars {"0123456789`~-=_+[]\\{}|;\':\",./<>?qwertyuiopasdfghjklzxcvbnm"};
    size_t len {chars.length()};
    for (int i {}; i < 10; ++i) {
        int c = rand() % 1000;
        std::string result;
        result.reserve(c);
        for (int j {}; j < c; ++j) {
            result += chars.at(rand() % len);
        }
        ds::str s {const_cast<char *>(result.c_str()), (ds::ull)c};
        ds::ull r1 {ds::default_hash(s)}, r2 {test_str.hash(s)};
        assert(r1 == r2);
    }

    return 0;
}
