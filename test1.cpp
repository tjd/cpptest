// test1.cpp

#include "cpptest.h"

using cpptest::master;
using cpptest::New_test;

int twice(int x) {
    return 2 * x;
}

void twice_test() {
    NEW_TEST("twice");
    // cpptest::New_test test("twice");
    // master.new_test("twice");

    // master.equal_int(4, twice(2));
    // master.not_equal_int(4, twice(3));
    EQUAL_INT(4, twice(2));
    EQUAL_INT(4, twice(3));
    
    // master.test_completed();
}

int main() {
    cout << "Testing cpptest ..." << endl;

    twice_test();

    master.display_all_stats();
}
