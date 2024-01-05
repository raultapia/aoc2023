#include <array>
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

inline std::array<long, 2> move(const std::array<long, 2> p, const char dir, const long n) {
  switch(dir) {
  case 'U':
  case '3':
    return {p[0] - n, p[1]};
  case 'D':
  case '1':
    return {p[0] + n, p[1]};
  case 'L':
  case '2':
    return {p[0], p[1] - n};
  case 'R':
  case '0':
    return {p[0], p[1] + n};
  default:
    return {0, 0};
  }
  return {0, 0};
}

inline unsigned long area(const std::vector<std::array<long, 2>> &points, const long total) {
  long ret = 0;
  for(std::size_t i = 1; i < points.size() - 1; i++) {
    ret += points[i][0] * (points[i + 1][1] - points[i - 1][1]);
  }
  return total + 1 - (ret + total) / 2;
}

unsigned long day18a(const std::string &filename) {
  std::ifstream file(filename);
  std::vector<std::array<long, 2>> points;
  std::array<long, 2> p{0, 0};

  char dir{0};
  long steps{0};
  long total = 0;
  while(file >> dir) {
    file >> steps;
    file.ignore(10);

    total += steps;
    points.emplace_back(p = move(p, dir, steps));
  }

  return area(points, total);
}

unsigned long day18b(const std::string &filename) {
  std::ifstream file(filename);
  std::vector<std::array<long, 2>> points;
  std::array<long, 2> p{0, 0};

  std::string hex;
  long total = 0;
  while(file >> hex) {
    if(hex.front() == '(') {
      const long steps = std::stoi(hex.substr(2, 5), nullptr, 16);
      total += steps;
      points.emplace_back(p = move(p, hex[7], steps));
    }
  }

  return area(points, total);
}
