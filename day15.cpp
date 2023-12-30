#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

inline unsigned char hash(const std::string &s) {
  return std::accumulate(s.begin(), s.end(), 0, [](unsigned char x, char c) { return (x + static_cast<unsigned long>(c)) * 17 % 256; });
}

unsigned long day15a(const std::string &filename) {
  std::ifstream file(filename);
  std::string sequence;
  std::string step;
  unsigned long result = 0;

  while(file >> sequence) {
    std::stringstream ss(sequence);
    while(std::getline(ss, step, ',')) {
      result += hash(step);
    }
  }

  return result;
}

unsigned long day15b(const std::string &filename) {
  std::ifstream file(filename);
  std::string sequence;
  std::string step;
  unsigned long result = 0;
  std::array<std::unordered_map<std::string, unsigned long>, 256> boxes;
  std::array<std::vector<std::string>, 256> sort;

  while(file >> sequence) {
    std::stringstream ss(sequence);
    while(std::getline(ss, step, ',')) {
      const std::size_t n = step.find_first_of("=-");
      const std::string key = step.substr(0, n);
      const unsigned long h = hash(key);

      if(step.at(n) == '-') {
        if(static_cast<bool>(boxes.at(h).erase(key))) {
          sort.at(h).erase(std::find(sort.at(h).begin(), sort.at(h).end(), key));
        }
      } else if(boxes.at(h).insert_or_assign(key, static_cast<unsigned long>(step.at(n + 1) - '0')).second) {
        sort.at(h).emplace_back(key);
      }
    }
  }

  for(std::size_t i = 1; i <= boxes.size(); i++) {
    const std::vector<std::string> &s = sort.at(i - 1);
    std::unordered_map<std::string, unsigned long> &b = boxes.at(i - 1);
    for(std::size_t j = 1; j <= s.size(); j++) {
      result += i * j * b[s[j - 1]];
    }
  }

  return result;
}
