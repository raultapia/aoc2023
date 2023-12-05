#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

unsigned long day05a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::pair<unsigned long, bool>> garden;

  file >> line;
  while(file >> line) {
    if(!static_cast<bool>(std::isdigit(line.front()))) {
      break;
    }
    garden.emplace_back(std::stoul(line), false);
  }

  while(file >> line) {
    if(static_cast<bool>(std::isdigit(line.front()))) {
      unsigned long x0{0}, y0{std::stoul(line)}, len{0};
      file >> x0;
      file >> len;
      for(std::pair<unsigned long, bool> &g : garden) {
        if(!g.second && g.first >= x0 && g.first <= x0 + len) {
          g.first = y0 + g.first - x0;
          g.second = true;
        }
      }
    } else if(line.back() == ':') {
      for(std::pair<unsigned long, bool> &g : garden) {
        g.second = false;
      }
    }
  }

  return std::min_element(garden.begin(), garden.end(), [](const std::pair<unsigned long, bool> &a, const std::pair<unsigned long, bool> &b) { return a.first < b.first; })->first;
}

unsigned long day05b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::string line2;
  std::vector<std::array<unsigned long, 2>> garden;
  std::vector<bool> changed;

  file >> line;
  while(file >> line) {
    if(!static_cast<bool>(std::isdigit(line.front()))) {
      break;
    }
    file >> line2;
    garden.emplace_back(std::array<unsigned long, 2>{std::stoul(line), std::stoul(line) + std::stoul(line2)});
    changed.emplace_back(false);
  }

  std::vector<std::vector<std::array<unsigned long, 3>>> maps;
  while(file >> line) {
    if(static_cast<bool>(std::isdigit(line.front()))) {
      unsigned long x0{0}, y0{std::stoul(line)}, len{0};
      file >> x0;
      file >> len;
      maps.back().emplace_back(std::array<unsigned long, 3>{x0, x0 + len - 1, y0});
    } else if(line.back() == ':') {
      maps.emplace_back();
    }
  }

  for(const std::vector<std::array<unsigned long, 3>> &map : maps) {
    std::fill(changed.begin(), changed.end(), false);
    for(const std::array<unsigned long, 3> &m : map) {
      for(std::size_t k = 0; k < garden.size(); k++) {
        const unsigned long x0 = m[0];
        const unsigned long x1 = m[1];
        const unsigned long y0 = m[2];
        if(changed[k] || (garden[k][0] < x0 && garden[k][1] < x0) || (garden[k][0] > x1 && garden[k][1] > x1)) {
          continue;
        }
        if(garden[k][0] >= x0 && garden[k][0] <= x1 && garden[k][1] <= x1) {
        } else if(garden[k][0] >= x0 && garden[k][0] <= x1 && garden[k][1] > x1) {
          garden.emplace_back(std::array<unsigned long, 2>{x1 + 1, garden[k][1]});
          changed.emplace_back(false);
          garden[k][1] = x1;
        } else if(garden[k][1] >= x0 && garden[k][1] <= x1) {
          garden.emplace_back(std::array<unsigned long, 2>{garden[k][0], x0 - 1});
          changed.emplace_back(false);
          garden[k][0] = x0;
        } else if(garden[k][0] < x0 && garden[k][1] > x1) {
          garden.emplace_back(std::array<unsigned long, 2>{garden[k][0], x0 - 1});
          garden.emplace_back(std::array<unsigned long, 2>{x1 + 1, garden[k][1]});
          changed.emplace_back(false);
          changed.emplace_back(false);
          garden[k][0] = x0;
          garden[k][1] = x1;
        }
        garden[k][0] = y0 + garden[k][0] - x0;
        garden[k][1] = y0 + garden[k][1] - x0;
        changed[k] = true;
      }
    }
  }

  return std::min_element(garden.begin(), garden.end(), [](const std::array<unsigned long, 2> &a, const std::array<unsigned long, 2> &b) { return a[0] < b[0]; })->at(0);
}
