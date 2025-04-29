#include "./finder.hpp"
#include "./hash.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: waddup <directory>" << "\n";
    return 1;
  }

  std::cout << "WAD DUPlicate detector" << "\n";

  waddup::Finder finder(argv[1]);
  auto           duplicates = finder.findDuplicates();

  for (const auto &line : duplicates) {
    std::cout << line << "\n";
  }

  return 0;
}
