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
    NEW_TEST("pluralize");

    EQUAL_STR("cats", pluralize("cats"));
    EQUAL_STR("cats", pluralize("cat"));
    EQUAL_STR("babies", pluralize("baby"));
}

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
    pluralize_test();

    // master.display_all_stats();
}
