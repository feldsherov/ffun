#include <iostream>
#include <set>

int main() {
  std::set<int> s{1, 2, 3, 4};
  if (const auto it = s.find(1); it != s.end()) {
    std::cerr << "ok"; 
  } else {
    std::cerr << "not ok";
  }
  return 0;
}
