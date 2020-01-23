#include <iostream>
#include <utility>
#include <string>

int main() {
  std::string empty_string;
  std::cerr << "sizeof(std::string) = " << sizeof(std::string) << std::endl;
  for (int32_t i = 0; i < 32; ++i) {
    std::cerr << "std::string(" << i << ", 'a').capacity()" << " = "
              << std::string(i, 'a').capacity() << std::endl;
    std::cerr << "std::string("<<  i << ", 'a').size()" << " = "
              << std::string(i, 'a').size() << std::endl;
  }

  return 0;
}
