#ifndef CPPTEST_H
#define CPPTEST_H

#include <iostream>
#include <sstream>
#include <map>
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

void error_if(bool b, const string& msg) {
  if (b) error(msg);
}

template<class T> string to_str(const T& x) {
  ostringstream os;
  os << x;
  return os.str();
}

template<class T> string to_pair(const T& a, const T& b) {
  return "(" + to_str(a) + ", " + to_str(b) + ")";
}

template<class T>
void print_msg(const string& msg, const string& result,
               const T& expected, const T& actual) {
      cout << "  " << result << ' ' << msg << ' '
           << to_pair(expected, actual) << endl;  
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
  bool completed;

  Test_suite() 
    : name("<test-with-no-name>"), attempted(0), passed(0), failed(0), 
      completed(false)
    { }

  Test_suite(const string& name) 
    : name(name), attempted(0), passed(0), failed(0), 
      completed(false)
    { } 
};

//////////////////////////////////////////////////////////////////////////
//
// Test master class.
//
//////////////////////////////////////////////////////////////////////////

class Test_master {
  map<string, Test_suite> stats;
  string active_test;

  void has_test(const string& name) {
    error_if(stats.count(name) == 0, "no such test: " + quote(name));
  }

  void lacks_test(const string& name) {
    error_if(stats.count(name) > 0, "duplicate test: " + quote(name));
  }

  string assert(const string& name, bool expr) {
    has_test(name);
    error_if(stats[name].completed, "test suite " + quote(name) + " completed");
    stats[name].attempted++;
    if (expr) {
      stats[name].passed++;
      return "PASSED";
    } else {
      stats[name].failed++;
      return "FAILED";
    }
  }

public:
  Test_master() : stats(), active_test("") { }

  void new_test(const string& name) {
    lacks_test(name);
    error_if(name == "", "test name cannot be empty string");
    stats.emplace(name, Test_suite(name));
    cout << "\nTest " << quote(name) << " created ..." << endl;
    active_test = name;
  }

  void test_completed() {
    has_active_test();
    test_completed(active_test);
  }

  void test_completed(const string& name) {
    has_test(name);
    int attempted = stats[name].attempted;
    int passed = stats[name].passed;
    int failed = stats[name].failed;
    stats[name].completed = true;
    printf("... test \"%s\" completed ", name.c_str());
    printf("(%d attempted, %d passed, %d failed)\n", attempted, passed, failed);
    active_test = "";
  }

  void has_active_test() {
    error_if(active_test == "", "no active test");
  }

  void is_true(bool expr) {
    has_active_test();
    is_true(active_test, expr); 
  }

  void is_true(const string& name, bool expr) {
    string result = assert(name, expr);
    cout << "  " << result << " is_true test " << endl;
  }

  void is_false(bool expr) {
    has_active_test();
    is_false(active_test, expr); 
  }

  void is_false(const string& name, bool expr) {
    string result = assert(name, !expr);
    cout << "  " << result << " is_false test " << endl;
  }

  ////////////////////////////////////////////////////////////////

  void equal_int(int expected, int actual) {
    has_active_test();
    equal_int(active_test, expected, actual); 
  }

  void equal_int(const string& name, int expected, int actual) {
    string result = assert(name, expected == actual);
    print_msg(string("int equality test"), result, expected, actual);
  }

  void not_equal_int(int expected, int actual) {
    has_active_test();
    not_equal_int(active_test, expected, actual); 
  }

  void not_equal_int(const string& name, int expected, int actual) {
    string result = assert(name, expected != actual);    
    print_msg(string("int inequality test"), result, expected, actual);    
  }

  ////////////////////////////////////////////////////////////////

  void equal_double(double expected, double actual) {
    has_active_test();
    equal_int(active_test, expected, actual); 
  }

  void equal_double(const string& name, double expected, double actual) {
    string result = assert(name, expected == actual);
    print_msg(string("double equality test"), result, expected, actual);
  }

  void not_equal_double(double expected, double actual) {
    has_active_test();
    not_equal_double(active_test, expected, actual); 
  }

  void not_equal_double(const string& name, double expected, double actual) {
    string result = assert(name, expected != actual);   
    print_msg(string("double inequality test"), result, expected, actual); 
  }

  ////////////////////////////////////////////////////////////////

  void equal_str(const string& expected, const string& actual) {
    has_active_test();
    equal_str(active_test, expected, actual); 
  }

  void equal_str(const string& name, const string& expected, const string& actual) {
    string result = assert(name, expected == actual);
    print_msg("string equality test", result, expected, actual); 
  }

  void not_equal_str(const string& name, const string& expected, const string& actual) {
    string result = assert(name, expected != actual);   
    print_msg("double inequality test", result, expected, actual); 
  }

  ////////////////////////////////////////////////////////////////

  void display_all_stats() {
    int passed = 0;
    int failed = 0;
    int attempted = 0;

    int suites_passed = 0;

    cout << "\n----------------------------------------------------------------\n"
         << " Final Summary of All Tests \n"
         << "----------------------------------------------------------------\n";

    for(auto& kv : stats) {
      Test_suite t = kv.second;

      double pass_pct = 100.0 * t.passed / t.attempted;
      printf("\"%s\" test suite: %d attempted, %d passed (%.1f%%), %d failed (%.1f%%)\n", 
             t.name.c_str(), t.attempted, t.passed, pass_pct, t.failed, 100 - pass_pct);
      
      passed += t.passed;
      failed += t.failed;
      attempted += t.attempted;

      if (t.failed == 0) {
        suites_passed++;
      }
    }

    cout << endl;
    double sp_pct = 100.0 * suites_passed / stats.size();
    printf("        Total suites attempted: %2d\n", stats.size());
    printf("Total suites completely passed: %2d (%.1f%%)\n", suites_passed, sp_pct);
    printf(" Total suites partially failed: %2d (%.1f%%)\n\n", 
                                                          stats.size() - suites_passed,
                                                          100 - sp_pct);

    double pass_pct = 100.0 * passed / attempted;    
    printf("         Total cases attempted: %2d\n", attempted);
    printf("            Total cases passed: %2d (%.1f%%)\n", passed, pass_pct);
    printf("            Total cases failed: %2d (%.1f%%)\n", failed, 100 - pass_pct);
  
    cout << endl;
  }

}; // class Test_master

} // namespace cpptest

#endif
