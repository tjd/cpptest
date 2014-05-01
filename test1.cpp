// test1.cpp

#include "cpptest.h"

cpptest::Test_master master;

int twice(int x) {
    return 2 * x;
}

void twice_test() {
    master.new_test("twice");

    master.equal_int(4, twice(2));
    master.not_equal_int(4, twice(3));
    
    master.test_completed();
}

int main() {
    cout << "Testing cpptest ..." << endl;

    twice_test();

    master.display_all_stats();
}
