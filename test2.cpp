// test2.cpp

#include "cpptest.h"

using cpptest::master;

string pluralize(const string& s) {
    const int n = s.size();
    if (n == 0) return s;

    char last = s.back();
    if (last == 's') return s;
    if (last == 'y') {
        string t = s.substr(0, n - 1);
        return t + "ies";
    }
    return s + "s";
}

void pluralize_test() {
    master.new_test("pluralize");

    master.equal_str("cats", pluralize("cats"));
    master.equal_str("cats", pluralize("cat"));
    master.equal_str("babies", pluralize("baby"));
    
    master.test_completed();
}

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
    pluralize_test();

    master.display_all_stats();
}
