#include <cassert>
#include "../include/data_structure.hpp"

int main() {

    ds::ipair<int, int> arguments [] {{1, 2}, {3, 4}, {5, 6}};
    ds::structure<int, int> test {ds::args(arguments)};

    ds::iterator<ds::pair<int, int>> iter {};
    
    try {
        iter.value();
        assert(false && "iter.value() should not succeed.");
    } catch (ds::exception::null_access &e) {
    }
    try {
        *iter;
        assert(false && "*iter should not succeed.");
    } catch (ds::exception::null_access &e) {
    }
    try {
        iter.hasNext();
        assert(false && "iter.hasNext() should not succeed.");
    } catch (ds::exception::null_access &e) {
    }
    try {
        iter.hasPrev();
        assert(false && "iter.hasPrev() should not succeed.");
    } catch (ds::exception::null_access &e) {
    }
    try {
        iter.next();
        assert(false && "iter.next() should not succeed.");
    } catch (ds::exception::null_access &e) {
    }
    try {
        iter.prev();
        assert(false && "iter.prev() should not succeed.");
    } catch (ds::exception::null_access &e) {
    }
    try {
        ++iter;
        iter++;
        assert(false && "iter increment should not succeed.");
    } catch (ds::exception::null_access &e) {
    }
    try {
        --iter;
        iter--;
        assert(false && "iter decrement should not succeed.");
    } catch (ds::exception::null_access &e) {
    }

    iter = test.linkedlist.begin();
    assert(iter.hasNext());
    assert(!iter.hasPrev());

    assert(iter.value().first() == 1);
    assert(iter.value().second() == 2);
    ++iter;
    assert(iter.value().first() == 3);
    assert(iter.value().second() == 4);
    ++iter;
    assert(iter.value().first() == 5);
    assert(iter.value().second() == 6);

    assert(iter.hasNext());
    assert(++iter == test.linkedlist.end());
    assert(!iter.hasNext());

    iter = test.linkedlist.end();

    assert(iter.hasPrev());
    assert(!iter.hasNext());
    assert(!iter.valid());
    --iter;

    assert(iter.value().first() == 5);
    assert(iter.value().second() == 6);
    --iter;
    assert(iter.value().first() == 3);
    assert(iter.value().second() == 4);
    --iter;
    assert(iter.value().first() == 1);
    assert(iter.value().second() == 2);

    assert(!iter.hasPrev());

    assert(iter.next().first() == 1);
    assert(iter.valid());

    return 0;
}
