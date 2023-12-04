#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <string>

int day01a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  int result = 0;

  while(file >> line) {
    const std::string::iterator it1 = std::find_if(line.begin(), line.end(), [](char ch) { return std::isdigit(ch); });
    const std::string::reverse_iterator it2 = std::find_if(line.rbegin(), line.rend(), [](char ch) { return std::isdigit(ch); });
    result += std::stoi(std::string(1, *it1) + std::string(1, *it2));
  }

  return result;
}

int day01b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  const std::array<std::string, 9> digits{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
  std::array<int, 9> position1{};
  std::array<int, 9> position2{};
  int result = 0;

  while(file >> line) {
    for(std::size_t i = 0; i < digits.size(); i++) {
      const std::size_t found1 = line.find(digits.at(i));
      const std::size_t found2 = line.rfind(digits.at(i));
      position1.at(i) = static_cast<int>((found1 != std::string::npos) ? found1 : -1);
      position2.at(i) = static_cast<int>((found2 != std::string::npos) ? found2 : -1);
    }

    const std::string::iterator it1 = std::find_if(line.begin(), line.end(), [](char ch) { return std::isdigit(ch); });
    const std::string::reverse_iterator it2 = std::find_if(line.rbegin(), line.rend(), [](char ch) { return std::isdigit(ch); });
    std::array<int, 9>::iterator it1b = std::min_element(position1.begin(), position1.end(), [](const int a, const int b) { return a >= 0 && (b < 0 || a < b); });
    std::array<int, 9>::iterator it2b = std::max_element(position2.begin(), position2.end());

    if(std::distance(line.begin(), it1) < *it1b || *it1b < 0) {
      result += std::stoi(std::string(1, *it1)) * 10;
    } else {
      result += static_cast<int>(std::distance(position1.begin(), it1b) * 10) + 10;
    }

    if(std::distance(line.rbegin(), it2) < static_cast<int>(line.size()) - *it2b - 1) {
      result += std::stoi(std::string(1, *it2));
    } else {
      result += static_cast<int>(std::distance(position2.begin(), it2b)) + 1;
    }
  }

  return result;
}
