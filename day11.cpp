#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

struct Point {
  int row = 0;
  int col = 0;
  [[nodiscard]] unsigned long distance(const Point &p, const std::array<std::vector<bool>, 2> &twice, const int n = 2) const {
    const Point a{std::min(row, p.row), std::min(col, p.col)};
    const Point b{std::max(row, p.row), std::max(col, p.col)};
    const long c1 = std::count(twice[0].begin() + a.row, twice[0].begin() + b.row + 1, true);
    const long c2 = std::count(twice[1].begin() + a.col, twice[1].begin() + b.col + 1, true);
    return (b.row - a.row) + (b.col - a.col) + (n - 1) * c1 + (n - 1) * c2;
  }
} __attribute__((aligned(8)));

unsigned long day11a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> universe;
  std::vector<Point> galaxies;
  unsigned long result = 0;

  file >> line;
  std::array<std::vector<bool>, 2> twice{std::vector<bool>(line.size(), true), std::vector<bool>(line.size(), true)};
  file.seekg(0, std::ios::beg);

  while(file >> line) {
    universe.emplace_back(line);
    for(std::string::iterator it = line.begin(); it != line.end(); ++it) {
      if(*it == '#') {
        galaxies.emplace_back(Point{static_cast<int>(universe.size() - 1), static_cast<int>(std::distance(line.begin(), it))});
        twice[0].at(universe.size() - 1) = false;
        twice[1].at(std::distance(line.begin(), it)) = false;
      }
    }
  }

  for(std::size_t i = 0; i < galaxies.size(); i++) {
    for(std::size_t j = i + 1; j < galaxies.size(); j++) {
      result += galaxies[i].distance(galaxies[j], twice);
    }
  }

  return result;
}

unsigned long day11b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> universe;
  std::vector<Point> galaxies;
  unsigned long result = 0;

  file >> line;
  std::array<std::vector<bool>, 2> twice{std::vector<bool>(line.size(), true), std::vector<bool>(line.size(), true)};
  file.seekg(0, std::ios::beg);

  while(file >> line) {
    universe.emplace_back(line);
    for(std::string::iterator it = line.begin(); it != line.end(); ++it) {
      if(*it == '#') {
        galaxies.emplace_back(Point{static_cast<int>(universe.size() - 1), static_cast<int>(std::distance(line.begin(), it))});
        twice[0].at(universe.size() - 1) = false;
        twice[1].at(std::distance(line.begin(), it)) = false;
      }
    }
  }

  for(std::size_t i = 0; i < galaxies.size(); i++) {
    for(std::size_t j = i + 1; j < galaxies.size(); j++) {
      result += galaxies[i].distance(galaxies[j], twice, 1000000);
    }
  }

  return result;
}
