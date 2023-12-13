#include <algorithm>
#include <cstddef>
#include <fstream>
#include <functional>
#include <numeric>
#include <string>
#include <vector>

inline unsigned int count_mismatches(const std::string &a, const std::string &b) {
  return std::inner_product(a.begin(), a.end(), b.begin(), 0, std::plus(), std::not_equal_to());
}

inline std::size_t find_reflection(const std::vector<std::string> &input) {
  for(long i = 1; i < static_cast<long>(input.size()); i++) {
    const std::string a = std::accumulate(input.rend() - i, input.rend(), std::string());
    const std::string b = std::accumulate(input.begin() + i, input.end(), std::string());
    const std::size_t m = std::min(a.size(), b.size());
    if(a.substr(0, m) == b.substr(0, m)) {
      return i;
    }
  }
  return 0;
}

inline std::size_t fix_reflection(const std::vector<std::string> &input) {
  for(long i = 1; i < static_cast<long>(input.size()); i++) {
    const std::string a = std::accumulate(input.rend() - i, input.rend(), std::string());
    const std::string b = std::accumulate(input.begin() + i, input.end(), std::string());
    const std::size_t m = std::min(a.size(), b.size());
    if(count_mismatches(a.substr(0, m), b.substr(0, m)) == 1) {
      return i;
    }
  }
  return 0;
}

unsigned long day13a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> horizontal;
  std::vector<std::string> vertical;
  unsigned long result = 0;

  while(std::getline(file, line)) {
    if(line.empty()) {
      result += 100 * find_reflection(horizontal) + find_reflection(vertical);
      horizontal.clear();
      vertical.clear();
      continue;
    }
    if(vertical.empty()) {
      vertical.resize(line.size());
    }
    horizontal.push_back(line);
    for(std::size_t i = 0; i < line.size(); i++) {
      vertical[i].push_back(line[i]);
    }
  }

  return result + 100 * find_reflection(horizontal) + find_reflection(vertical);
}

unsigned long day13b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> horizontal;
  std::vector<std::string> vertical;
  unsigned long result = 0;

  while(std::getline(file, line)) {
    if(line.empty()) {
      result += 100 * fix_reflection(horizontal) + fix_reflection(vertical);
      horizontal.clear();
      vertical.clear();
      continue;
    }
    if(vertical.empty()) {
      vertical.resize(line.size());
    }
    horizontal.push_back(line);
    for(std::size_t i = 0; i < line.size(); i++) {
      vertical[i].push_back(line[i]);
    }
  }

  return result + 100 * fix_reflection(horizontal) + fix_reflection(vertical);
}
