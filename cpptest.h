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

// template<class T>
// void print_msg(const string& msg, const string& result,
//                const T& expected, const T& actual) {
//       cout << "  " << result << ' ' << msg << ' '
//            << to_pair(expected, actual) << endl;  
// }

void print_msg(const string& result) {
      cout << " " << result << endl;  
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

// Trace is used to write simple debugging messages when a function is
// called and when it ends. Indentation is used to align a function's
// entering/exiting 
//
// To use it, put it as the first statement
// of a function. For example:
//
//  int sum(int n) {
//    Trace trace("sum(" + to_string(n) + ")");
//    if (n <= 0) 
//      return 0;
//    else
//      return n + sum(n - 1);
//  }
//
// When sum(5) is called, this output is printed:
//
//  sum(5) entered ...
//    sum(4) entered ...
//      sum(3) entered ...
//        sum(2) entered ...
//          sum(1) entered ...
//            sum(0) entered ...
//            ... sum(0) exited
//          ... sum(1) exited
//        ... sum(2) exited
//      ... sum(3) exited
//    ... sum(4) exited
//  ... sum(5) exited
//  
class Trace {
  string msg;
  string spaces;
  static int indent;
  const static int indent_increase;

 public:
  Trace(const string& prompt) : msg(prompt), spaces(indent, ' ') {
    cout << spaces << msg << " entered ..." << endl;
    indent += indent_increase;
  }

  ~Trace() {
    cout << spaces << "... " << msg << " exited" << endl;
    indent -= indent_increase;
  }
}; // class Trace

// The variables indent and indent_increase are static, meaning there
// is only one copy of each that is shared by all Trace objects. 
int Trace::indent = 0;
const int Trace::indent_increase = 2;

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
  bool completed;

  Test_suite() 
    : name("<test-with-no-name>"), attempted(0), passed(0), failed(0), 
      excepted(0), completed(false)
    { }

  Test_suite(const string& name) 
    : name(name), attempted(0), passed(0), failed(0), 
      excepted(0), completed(false)
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
    Trace trace("equal_int_1" + to_pair(expected, actual));
    has_active_test();
    equal_int(active_test, expected, actual); 
  }

  void equal_int(const string& name, int expected, int actual) {
    Trace trace("equal_int_2" + to_pair(expected, actual));
    string result = assert(name, expected == actual);
    print_msg(result);
  }

  void not_equal_int(int expected, int actual) {
    has_active_test();
    not_equal_int(active_test, expected, actual); 
  }

  void not_equal_int(const string& name, int expected, int actual) {
    string result = assert(name, expected != actual);    
    print_msg(result);    
  }

  ////////////////////////////////////////////////////////////////

  // void equal_double(double expected, double actual) {
  //   has_active_test();
  //   equal_double(active_test, expected, actual); 
  // }

  // void equal_double(const string& name, double expected, double actual) {
  //   string result = assert(name, expected == actual);
  //   print_msg("double equality test", result, expected, actual);
  // }

  // void not_equal_double(double expected, double actual) {
  //   has_active_test();
  //   not_equal_double(active_test, expected, actual); 
  // }

  // void not_equal_double(const string& name, double expected, double actual) {
  //   string result = assert(name, expected != actual);   
  //   print_msg("double inequality test", result, expected, actual); 
  // }

  ////////////////////////////////////////////////////////////////

  // void equal_str(const string& expected, const string& actual) {
  //   has_active_test();
  //   equal_str(active_test, expected, actual); 
  // }

  // void equal_str(const string& name, const string& expected, const string& actual) {
  //   string result = assert(name, expected == actual);
  //   print_msg("string equality test", result, expected, actual); 
  // }

  // void not_equal_str(const string& name, const string& expected, const string& actual) {
  //   string result = assert(name, expected != actual);   
  //   print_msg("double inequality test", result, expected, actual); 
  // }

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

Test_master master;

class New_test {
  string name;

public:
  New_test(const string& name) {
    master.new_test(name);
    cout << "New_test(" << name << ") created ..." << endl;
  }

  ~New_test() {
    master.test_completed();
    cout << "... New_test(" << name << ") completed" << endl;
  }
}; // class New_test

//
// Macros
//

#define NEW_TEST(name)  \
{                       \
  New_test test(name);  \
}

#define EQUAL_INT(expected, actual)              \
{                                                \
  try {                                          \
    cout << "  EQUAL_INT(" << #expected << ", " << #actual << "): ";  \
    master.equal_int(expected, actual);          \
  } catch (...) {                                \
    cout << "EXCEPTION\n";                       \
  }                                              \
}

    /*cout << "EQUAL_INT(" << #expected << ", "    \
                         << #actual << ")" << endl; \*/

} // namespace cpptest

#endif
