#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

inline std::vector<std::string> split(const std::string &s) {
  std::vector<std::string> r(1);
  for(const char c : s) {
    (c == '#') ? r.emplace_back() : r.back() += c;
  }
  return r;
}

inline void tilt(std::vector<std::string> &x, const bool move = true) {
  const std::size_t n = x.size();
  for(std::size_t i = 0; i < n; i++) {
    for(std::size_t j = i + 1; j < n; j++) {
      std::swap(x[i][j], x[j][i]);
    }
  }
  for(std::string &p : x) {
    std::reverse(p.begin(), p.end());
    if(move) {
      std::string aux;
      for(const std::string &s : split(p)) {
        aux += std::string(std::count(s.begin(), s.end(), '.'), '.') + std::string(std::count(s.begin(), s.end(), 'O'), 'O') + '#';
      }
      p = aux.substr(0, aux.size() - 1);
    }
  }
}

inline unsigned long compute(const std::vector<std::string> &x) {
  unsigned long sum = 0;
  for(const std::string &s : x) {
    for(std::size_t k = 0; k < s.size(); k++) {
      if(s[k] == 'O') {
        sum += (k + 1);
      }
    }
  }
  return sum;
}

inline std::string concat(const std::vector<std::string> &x) {
  return std::accumulate(x.begin(), x.end(), std::string());
}

unsigned long day14a(const std::string &filename) {
  std::ifstream file(filename);
  std::vector<std::string> platform{std::istream_iterator<std::string>(file), std::istream_iterator<std::string>()};
  tilt(platform);
  return compute(platform);
}

unsigned long day14b(const std::string &filename) {
  std::ifstream file(filename);
  std::vector<std::string> platform{std::istream_iterator<std::string>(file), std::istream_iterator<std::string>()};

  std::unordered_set<std::string> ref{concat(platform)};
  std::vector<std::vector<std::string>> hist{platform};
  unsigned long cycle = 0;
  while(++cycle <= 1000000000) {
    tilt(platform);
    tilt(platform);
    tilt(platform);
    tilt(platform);
    const std::string key = concat(platform);
    if(ref.find(key) != ref.end()) {
      break;
    }
    ref.insert(key);
    hist.emplace_back(platform);
  }
  const long index = std::distance(hist.begin(), std::find(hist.begin(), hist.end(), platform));
  platform = hist.at((1000000000 - index) % (cycle - index) + index);
  tilt(platform, false);

  return compute(platform);
}
