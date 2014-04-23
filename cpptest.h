#ifndef cpptest
#define cpptest

#include <iostream>
#include <sstream>
#include <map>
#include <stdio.h>

//
// TODO: Add a "completed" flag to Test_suite? Might help avoid errors due to
//       testing a completed suite.
// TODO: Add has_active_test to Test_master to throw an error if there is 
//       no active test? Might produce nicer error messages.
//

using namespace std;

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

template<class T> string to_pair(T a, T b) {
  return "(" + to_str(a) + ", " + to_str(b) + ")";
}

string quote(const string& x) {
  return "\"" + x + "\"";
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

  Test_suite() 
    : name("<test-with-no-name>"), attempted(0), passed(0), failed(0)
    { }

  Test_suite(const string& name) 
    : name(name), attempted(0), passed(0), failed(0) 
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
    test_completed(active_test);
  }

  void test_completed(const string& name) {
    has_test(name);
    int attempted = stats[name].attempted;
    int passed = stats[name].passed;
    int failed = stats[name].failed;
    printf("... test \"%s\" completed", name.c_str());
    printf("(%d attempted, %d passed, %d failed)\n", attempted, passed, failed);
    active_test = "";
  }

  void is_true(bool expr) {
    is_true(active_test, expr); 
  }

  void is_true(const string& name, bool expr) {
    has_test(name);
    string result;
    stats[name].attempted++;
    if (expr) {
      stats[name].passed++;
      result = "PASSED";
    } else {
      stats[name].failed++;
      result = "FAILED";
    }
    cout << "  " << result << " is_true test " << endl;
  }

  void equal(int expected, int actual) {
    equal(active_test, expected, actual); 
  }

  void equal(const string& name, int expected, int actual) {
    has_test(name);
    string result;
    stats[name].attempted++;
    if (expected == actual) {
      stats[name].passed++;
      result = "PASSED";
    } else {
      stats[name].failed++;
      result = "FAILED";
    }
    cout << "  " << result << " equality test " << to_pair(expected, actual) << endl;
  }

  void not_equal(int expected, int actual) {
    not_equal(active_test, expected, actual); 
  }

  void not_equal(const string& name, int expected, int actual) {
    has_test(name);

    string result;
    stats[name].attempted++;
    if (expected != actual) {
      stats[name].passed++;
      result = "PASSED";
    } else {
      stats[name].failed++;
      result = "FAILED";    
    }
      cout << "  " << result << " equality test " << to_pair(expected, actual) << endl;
  }

  void equal_str(const string& expected, const string& actual) { 
    equal_str(active_test, expected, actual); 
  }

  void equal_str(const string& name, const string& expected, const string& actual) {
    has_test(name);
    string result;
    stats[name].attempted++;
    if (expected == actual) {
      stats[name].passed++;
      result = "PASSED";
    } else {
      stats[name].failed++;
      result = "FAILED";
    }
    cout << "  " << result << " string equality test " 
         << to_pair(quote(expected), quote(actual)) << endl;
  }

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

#endif
