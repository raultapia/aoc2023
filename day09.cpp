#include <algorithm>
#include <fstream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

int day09a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<int> history;
  int result = 0;

  while(std::getline(file, line)) {
    std::istringstream iss(line);
    int n = 0;
    history.clear();
    while(iss >> n) {
      history.push_back(n);
    }
    if(history.empty()) {
      break;
    }

    n = static_cast<int>(history.size());
    while(std::any_of(history.begin(), history.begin() + (n--), [](int h) { return static_cast<bool>(h); })) {
      for(std::vector<int>::iterator it = history.begin(); it != history.begin() + n; ++it) {
        *it = *(it + 1) - *it;
      }
    }
    result += std::accumulate(history.begin(), history.end(), 0);
  }

  return result;
}

int day09b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<int> history;
  int result = 0;

  while(std::getline(file, line)) {
    std::istringstream iss(line);
    int n = 0;
    history.clear();
    while(iss >> n) {
      history.insert(history.begin(), n);
    }
    if(history.empty()) {
      break;
    }

    n = static_cast<int>(history.size());
    while(std::any_of(history.begin(), history.begin() + (n--), [](int h) { return static_cast<bool>(h); })) {
      for(std::vector<int>::iterator it = history.begin(); it != history.begin() + n; ++it) {
        *it = *(it + 1) - *it;
      }
    }
    result += std::accumulate(history.begin(), history.end(), 0);
  }

  return result;
}
