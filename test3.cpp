// test3.cpp

#include "cpptest.h"

double safe_invert(double x) {
    if (x == 0.0) cpptest::error("safe_invert: can't invert 0");
    return 1.0 / x;
}

void safe_invert_test() {
    NEW_TEST("safe_invert");
    
    EQUAL_DOUBLE(0.5, safe_invert(2));
    EQUAL_DOUBLE(0.3, safe_invert(3));
    EQUAL_DOUBLE(0.3333333, safe_invert(3));
    EQUAL_DOUBLE(0.0, safe_invert(0));
    IS_TRUE(safe_invert(2) == 0.5);
}

string pluralize(const string& s) {
    int n = s.size();
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
    safe_invert_test();

    cpptest::display_all_stats();
}
