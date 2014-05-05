// test1.cpp

#include "cpptest.h"

int twice(int x) {
    if (x == -1) cpptest::error("can't call twice on -1");
    return 2 * x;
}

void twice_test() {
    NEW_TEST("twice");

    EQUAL_INT(4, twice(2));
    EQUAL_INT(4, twice(3));
    EQUAL_INT(1, twice(-1));
}

int main() {
    cout << "Testing cpptest ..." << endl;

    twice_test();

    cpptest::display_all_stats();
}
