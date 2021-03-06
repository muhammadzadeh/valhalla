#include "baldr/streetnames.h"
#include "baldr/streetname.h"
#include "test.h"

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
using namespace valhalla::baldr;

namespace {

void TryListCtor(const std::vector<std::pair<std::string, bool>>& names) {
  StreetNames street_names(names);

  int x = 0;
  for (const auto& street_name : street_names) {
    if (names.at(x).first != street_name->value())
      throw std::runtime_error("Incorrect street name value");
    if (names.at(x).second != street_name->is_route_number())
      throw std::runtime_error("Incorrect street name is_route_number");
    ++x;
  }
}

void TestListCtor() {
  TryListCtor({{"Main Street", false}});
  TryListCtor({{"Hershey Road", false}, {"PA 743 North", true}});
  TryListCtor({{"Unter den Linden", false}, {"B 2", true}, {"B 5", true}});
}

void TryFindCommonStreetNames(const StreetNames& lhs,
                              const StreetNames& rhs,
                              const StreetNames& expected) {
  std::unique_ptr<StreetNames> computed = lhs.FindCommonStreetNames(rhs);
  if (computed->ToString() != expected.ToString()) {
    throw std::runtime_error(expected.ToString() +
                             ": Incorrect street names returned from FindCommonStreetNames");
  }
}

void TestFindCommonStreetNames() {
  TryFindCommonStreetNames(StreetNames({{"Hershey Road", false}, {"PA 743 North", true}}),
                           StreetNames({{"Fishburn Road", false}, {"PA 743 North", true}}),
                           StreetNames({{"PA 743 North", true}}));

  TryFindCommonStreetNames(StreetNames({{"Hershey Road", false}, {"PA 743 North", true}}),
                           StreetNames({{"Fishburn Road", false}, {"PA 743", true}}), StreetNames());

  TryFindCommonStreetNames(StreetNames({{"Capital Beltway", false},
                                        {"I 95 South", true},
                                        {"I 495 South", true}}),
                           StreetNames({{"I 95 South", true}}), StreetNames({{"I 95 South", true}}));

  TryFindCommonStreetNames(StreetNames({{"Unter den Linden", false}, {"B 2", true}, {"B 5", true}}),
                           StreetNames({{"B 2", true}, {"B 5", true}}),
                           StreetNames({{"B 2", true}, {"B 5", true}}));
}

void TryFindCommonBaseNames(const StreetNames& lhs,
                            const StreetNames& rhs,
                            const StreetNames& expected) {
  std::unique_ptr<StreetNames> computed = lhs.FindCommonBaseNames(rhs);
  if (computed->ToString() != expected.ToString()) {
    throw std::runtime_error(expected.ToString() +
                             ": Incorrect street names returned from FindCommonBaseNames");
  }
}

void TestFindCommonBaseNames() {
  TryFindCommonBaseNames(StreetNames({{"Hershey Road", false}, {"PA 743 North", true}}),
                         StreetNames({{"Fishburn Road", false}, {"PA 743 North", true}}),
                         StreetNames({{"PA 743 North", true}}));

  TryFindCommonBaseNames(StreetNames({{"Unter den Linden", false}, {"B 2", true}, {"B 5", true}}),
                         StreetNames({{"B 2", true}, {"B 5", true}}),
                         StreetNames({{"B 2", true}, {"B 5", true}}));
}

} // namespace

int main() {
  test::suite suite("streetnames");

  // Constructor with list argument
  suite.test(TEST_CASE(TestListCtor));

  // FindCommonStreetNames
  suite.test(TEST_CASE(TestFindCommonStreetNames));

  // FindCommonBaseNames
  suite.test(TEST_CASE(TestFindCommonBaseNames));

  return suite.tear_down();
}
