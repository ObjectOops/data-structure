#include <cassert>
#include "../include/data_structure.hpp"

int main() {

    ds::ipair<int, int> arguments [] {{1, 2}, {3, 4}, {5, 6}};
    ds::structure<int, int> test {ds::args(arguments)};

    ds::structure<int, int>::iterator iter {};
    
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

    assert(iter.value().getFirst() == 1);
    assert(iter.value().getSecond() == 2);
    ++iter;
    assert(iter.value().getFirst() == 3);
    assert(iter.value().getSecond() == 4);
    ++iter;
    assert(iter.value().getFirst() == 5);
    assert(iter.value().getSecond() == 6);

    assert(!iter.hasNext());

    iter = test.linkedlist.end();

    assert(iter.hasPrev());
    assert(!iter.hasNext());

    assert(iter.value().getFirst() == 5);
    assert(iter.value().getSecond() == 6);
    --iter;
    assert(iter.value().getFirst() == 3);
    assert(iter.value().getSecond() == 4);
    --iter;
    assert(iter.value().getFirst() == 1);
    assert(iter.value().getSecond() == 2);

    assert(!iter.hasPrev());

    assert(iter.next().getFirst() == 3);
    assert(iter.prev().getFirst() == 1);

    return 0;
}
