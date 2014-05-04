#ifndef CPPTEST_H
#define CPPTEST_H

#include <iostream>
#include <sstream>
#include <map>
#include <cmath>
#include <stdio.h>

using namespace std;

namespace cpptest {

//////////////////////////////////////////////////////////////////////////
//
// Helper functions.
//
//////////////////////////////////////////////////////////////////////////

void error(const string& msg) {
  throw runtime_error(msg);
}

template<class T> string to_str(const T& x) {
  ostringstream os;
  os << x;
  return os.str();
}

template<class T> string to_pair(const T& a, const T& b) {
  return "(" + to_str(a) + ", " + to_str(b) + ")";
}

string quote(const string& x) {
  return "\"" + x + "\"";
}

// Initialize the random number generator with a random seed based on the
// current time.
void randomize() {
  srand(time(NULL));
}

// Returns a random int n such that 0 <= n < max. It's important to note that
// the returned number will never be equal to max. For example, the biggest
// value randint(6) will return is 5 (not 6).
int randint(int max) { 
  return rand() % max; 
}

// Returns a random int n such that min <= n < max It's important to note that
// the returned number might be equal to min, but will never be equal to max.
// For example, randint(5, 10) might return 5, but it would never return 10
// (the biggest value it can return is 9).
int randint(int min, int max) { 
  return randint(max - min) + min; 
}

//////////////////////////////////////////////////////////////////////////
//
// Test suite class.
//
//////////////////////////////////////////////////////////////////////////

class Test_suite {
public:
  const string name;
  int attempted;
  int passed;
  int failed;
  int excepted;

  Test_suite() 
    : name("<test-with-no-name>"), attempted(0), passed(0), failed(0), 
      excepted(0)
    { }

  Test_suite(const string& name) 
    : name(name), attempted(0), passed(0), failed(0), 
      excepted(0)
    { } 
};

//////////////////////////////////////////////////////////////////////////
//
// Test master class.
//
//////////////////////////////////////////////////////////////////////////

map<string, Test_suite> master;
string current_test;

const double delta = 0.00001;

bool lacks_test(const string& name) { return master.count(name) == 0; }
bool has_test(const string& name) { return !lacks_test(name); }

void new_test(const string& name) {
  if (has_test(name)) {
    error("Already have test suite named " + quote(name));
  }
  current_test = name;
  master.emplace(name, Test_suite(name));
  cout << "\nTesting " << quote(name) << " ..." << endl;
}

class New_test {
  string name;
public:
  New_test(const string& name) : name(name) {
    new_test(name);
  }

  ~New_test() {
    cpptest::Test_suite& ts = cpptest::master[name];
    cout << "... " << quote(name) << " testing completed" 
         << " ("
         << ts.passed << " passed, "
         << ts.failed << " failed, "
         << ts.excepted << " excepted"
         << ")"
         << endl;
    current_test = "";
  }
};

#define NEW_TEST(name) cpptest::New_test test(name); 

#define EQUAL_INT(expected, actual)              \
{                                                \
  if (cpptest::current_test.empty()) cpptest::error("no current test");  \
  cpptest::Test_suite& ts = cpptest::master[cpptest::current_test];      \
  ts.attempted++;                                \
  string result;                                 \
  try {                                          \
    if (expected == actual) {                    \
      result = "   PASSED";                      \
      ts.passed++;                               \
    } else {                                     \
      result = "   FAILED";                      \
      ts.failed++;                               \
    }                                            \
  } catch (...) {                                \
    result = "EXCEPTION";                        \
    ts.excepted++;                               \
  }                                              \
    cout << "   " << ts.attempted << ". "        \
         << result                               \
         << ": EQUAL_INT(" << #expected << ", " << #actual << ")"    \
         << endl;                                \
}

#define EQUAL_STR(expected, actual)              \
{                                                \
  if (cpptest::current_test.empty()) cpptest::error("no current test");  \
  cpptest::Test_suite& ts = cpptest::master[cpptest::current_test];      \
  ts.attempted++;                                      \
  string result;                                 \
  try {                                          \
    cout << "   " << ts.attempted << ". ";             \
    cout << "EQUAL_STR(" << #expected << ", " << #actual << "): ";    \
    if (expected == actual) {                    \
      result = "PASSED";                         \
      ts.passed++;                               \
    } else {                                     \
      result = "FAILED";                         \
      ts.failed++;                               \
    }                                            \
  } catch (...) {                                \
    result = "EXCEPTION";                        \
    ts.excepted++;                               \
  }                                              \
  cout << result << endl;                        \
}

#define EQUAL_DOUBLE(expected, actual)           \
{                                                \
  if (cpptest::current_test.empty()) cpptest::error("no current test");  \
  cpptest::Test_suite& ts = cpptest::master[cpptest::current_test];      \
  ts.attempted++;                                \
  string result;                                 \
  try {                                          \
    cout << "   " << ts.attempted << ". ";       \
    cout << "EQUAL_DOUBLE(" << #expected << ", " << #actual << "): ";    \
    double diff = fabs(expected - actual);       \
    if (diff < cpptest::delta) {                 \
      result = "PASSED";                         \
      ts.passed++;                               \
    } else {                                     \
      result = "FAILED";                         \
      ts.failed++;                               \
    }                                            \
  } catch (...) {                                \
    result = "EXCEPTION";                        \
    ts.excepted++;                               \
  }                                              \
  cout << result << endl;                        \
}

#define IS_TRUE(actual)                          \
{                                                \
  if (cpptest::current_test.empty()) cpptest::error("no current test");  \
  cpptest::Test_suite& ts = cpptest::master[cpptest::current_test];      \
  ts.attempted++;                                \
  string result;                                 \
  try {                                          \
    cout << "   " << ts.attempted << ". ";       \
    cout << "IS_TRUE(" << #actual << "): ";      \
    result = actual ? "PASSED" : "FAILED";       \
    if (actual) {                                \
      result = "PASSED";                         \
      ts.passed++;                               \
    } else {                                     \
      result = "FAILED";                         \
      ts.failed++;                               \
    }                                            \
  } catch (...) {                                \
    result = "EXCEPTION";                        \
    ts.excepted++;                               \
  }                                              \
  cout << result << endl;                        \
}


void display_all_stats() {
  cout << "\n----------------------------------------------------------------\n"
       << " Final Summary of All Tests \n"
       << "----------------------------------------------------------------\n";

  int attempted = 0;
  int passed = 0;
  int failed = 0;
  int excepted = 0;
  int suites_passed = 0;

  for(auto& kv : master) {
    Test_suite t = kv.second;

    double pass_pct = 100.0 * t.passed / t.attempted;
    double except_pct = 100.0 * t.excepted / t.attempted;
    printf("\"%s\": %d attempted\n    %d passed (%.1f%%), %d failed (%.1f%%), %d exceptions (%.1f%%)\n", 
           t.name.c_str(), t.attempted, 
           t.passed, pass_pct, 
           t.failed, 100 - pass_pct,
           t.excepted, except_pct);
    
    passed += t.passed;
    failed += t.failed;
    attempted += t.attempted;
    excepted += t.excepted;

    if (t.failed == 0) {
      suites_passed++;
    }
  }

  cout << endl;
  double sp_pct = 100.0 * suites_passed / master.size();
  printf("        Total suites attempted: %2d\n", master.size());
  printf("Total suites completely passed: %2d (%.1f%%)\n", suites_passed, sp_pct);
  printf(" Total suites partially failed: %2d (%.1f%%)\n\n", 
                                          master.size() - suites_passed,
                                          100 - sp_pct);

  double pass_pct = 100.0 * passed / attempted;    
  printf("         Total cases attempted: %2d\n", attempted);
  printf("            Total cases passed: %2d (%.1f%%)\n", passed, pass_pct);
  printf("            Total cases failed: %2d (%.1f%%)\n", failed, 100 - pass_pct);

  cout << endl;
}

} // namespace cpptest

#endif
