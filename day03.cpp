#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

int day03a(const std::string &filename) {
  std::ifstream file(filename);
  std::string current;
  std::string prev;
  std::string next;
  std::array<int, 2> p{0};
  bool stop = false;
  int result = 0;

  while(true) {
    if(file >> next) {
      next.insert(next.begin(), '.');
      next.push_back('.');
    } else {
      stop = true;
      next = std::string(next.size(), '.');
    }

    if(prev.empty()) {
      prev = std::string(next.size(), '.');
      current = next;
      continue;
    }

    for(std::size_t k = 1; k < current.size() - 1; k++) {
      if(static_cast<bool>(std::isdigit(current[k])) && !static_cast<bool>(std::isdigit(current[k - 1]))) {
        p[0] = static_cast<int>(k);
      }
      if(static_cast<bool>(std::isdigit(current[k])) && !static_cast<bool>(std::isdigit(current[k + 1]))) {
        p[1] = static_cast<int>(k);
      }
      if(p[0] > 0 && p[1] > 0) {
        bool valid = (current[p[0] - 1] != '.' || current[p[1] + 1] != '.');
        for(int i = p[0] - 1; i <= p[1] + 1 && !valid; i++) {
          valid = (prev[i] != '.' || next[i] != '.');
        }
        if(valid) {
          result += std::stoi(current.substr(p[0], p[1] - p[0] + 1));
        }
        p[0] = p[1] = 0;
      }
    }

    if(stop) {
      break;
    }
    prev = current;
    current = next;
  }

  return result;
}

struct Gear {
  int row;
  int col;
  int ratio;

  bool operator==(const Gear &x) const {
    return row == x.row && col == x.col;
  }
} __attribute__((aligned(16)));

int day03b(const std::string &filename) {
  std::ifstream file(filename);
  std::string current;
  std::string prev;
  std::string next;
  std::array<int, 2> p{0};
  std::vector<Gear> gears;
  int iter = 0;
  bool stop = false;
  int result = 0;

  while(true) {
    iter++;
    if(file >> next) {
      next.insert(next.begin(), '.');
      next.push_back('.');
    } else {
      stop = true;
      next = std::string(next.size(), '.');
    }

    if(prev.empty()) {
      prev = std::string(next.size(), '.');
      current = next;
      continue;
    }

    for(std::size_t k = 1; k < current.size() - 1; k++) {
      if(static_cast<bool>(std::isdigit(current[k])) && !static_cast<bool>(std::isdigit(current[k - 1]))) {
        p[0] = static_cast<int>(k);
      }
      if(static_cast<bool>(std::isdigit(current[k])) && !static_cast<bool>(std::isdigit(current[k + 1]))) {
        p[1] = static_cast<int>(k);
      }
      if(p[0] > 0 && p[1] > 0) {
        bool valid = false;
        int row = 0;
        int col = 0;
        for(col = p[0] - 1; col <= p[1] + 1 && !valid; col++) {
          if(prev[col] == '*') {
            row = iter - 1;
            valid = true;
          } else if(current[col] == '*') {
            row = iter;
            valid = true;
          } else if(next[col] == '*') {
            row = iter + 1;
            valid = true;
          }
        }
        if(valid) {
          const Gear g{row, col, std::stoi(current.substr(p[0], p[1] - p[0] + 1))};
          const std::vector<Gear>::iterator it = std::find(gears.begin(), gears.end(), g);
          if(it == gears.end()) {
            gears.push_back(g);
          } else {
            result += it->ratio * g.ratio;
          }
        }
        p[0] = p[1] = 0;
      }
    }

    if(stop) {
      break;
    }
    prev = current;
    current = next;
  }

  return result;
}
