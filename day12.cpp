#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct CacheHash {
  std::size_t operator()(const std::pair<std::string, std::vector<unsigned long>> &x) const {
    const std::hash<std::string> hasher;
    std::string s(x.first);
    std::transform(x.second.begin(), x.second.end(), std::back_inserter(s), [](unsigned long x) { return static_cast<char>(x); });
    return hasher(s);
  }
};

inline std::vector<unsigned long> get_numbers(const std::string &str) {
  std::istringstream iss(str);
  std::vector<unsigned long> ret;
  unsigned long n{0};
  while(iss >> n) {
    ret.push_back(n);
    if(iss.peek() == ',') {
      iss.ignore();
    }
  }
  return ret;
}

unsigned long recursive_count(const std::string &springs, const std::vector<unsigned long> &numbers, std::unordered_map<std::pair<std::string, std::vector<unsigned long>>, unsigned long, CacheHash> &cache) {
  if(springs.empty()) {
    return static_cast<unsigned long>(numbers.empty());
  }
  if(numbers.empty()) {
    return static_cast<unsigned long>(springs.find('#') == std::string::npos);
  }
  if(const auto it = cache.find({springs, numbers}); it != cache.end()) {
    return it->second;
  }

  unsigned long ret = 0;
  if(springs.front() == '.' || springs.front() == '?') {
    ret += recursive_count(springs.substr(1), numbers, cache);
  }
  if(springs.front() == '#' || springs.front() == '?') {
    const std::size_t n{numbers[0]};
    const std::string ss{springs.substr(0, n)};
    if(n <= springs.size() && ss.find('.') == std::string::npos && (n == springs.size() || springs[n] != '#')) {
      ret += recursive_count((static_cast<std::size_t>(numbers[0] + 1) > springs.size()) ? "" : springs.substr(numbers[0] + 1), std::vector<unsigned long>(numbers.begin() + 1, numbers.end()), cache);
    }
  }

  cache.emplace(std::pair<std::string, std::vector<unsigned long>>{springs, numbers}, ret);
  return ret;
}

unsigned long day12a(const std::string &filename) {
  std::ifstream file(filename);
  std::string springs;
  std::string numbers;
  std::unordered_map<std::pair<std::string, std::vector<unsigned long>>, unsigned long, CacheHash> cache;
  unsigned long result = 0;

  while(file >> springs) {
    file >> numbers;
    result += recursive_count(springs, get_numbers(numbers), cache);
  }

  return result;
}

unsigned long day12b(const std::string &filename) {
  std::ifstream file(filename);
  std::string springs_folded;
  std::string numbers_folded;
  std::unordered_map<std::pair<std::string, std::vector<unsigned long>>, unsigned long, CacheHash> cache;
  unsigned long result = 0;

  while(file >> springs_folded) {
    file >> numbers_folded;

    std::string springs(springs_folded);
    std::string numbers(numbers_folded);
    for(int i = 0; i < 4; i++) {
      springs += "?" + springs_folded;
      numbers += "," + numbers_folded;
    }

    result += recursive_count(springs, get_numbers(numbers), cache);
  }

  return result;
}
