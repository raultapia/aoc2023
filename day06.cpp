#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int day06a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<int> time;
  std::vector<int> distance;
  int result = 1;

  while(std::getline(file, line)) {
    std::istringstream iss(line);
    std::string key;
    int value{0};
    iss >> key;
    while(iss >> value) {
      if(key.front() == 'T') {
        time.push_back(value);
      } else {
        distance.push_back(value);
      }
    }
  }

  for(std::size_t k = 0; k < time.size(); k++) {
    const int root1 = static_cast<int>((time[k] - std::sqrt(time[k] * time[k] - 4 * distance[k])) / 2.0);
    const int root2 = static_cast<int>((time[k] + std::sqrt(time[k] * time[k] - 4 * distance[k])) / 2.0);
    result *= root2 - root1 - static_cast<int>(root2 * (time[k] - root2) == distance[k]);
  }

  return result;
}

unsigned long day06b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;

  std::getline(file, line);
  line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char c) { return !static_cast<bool>(std::isdigit(c)); }), line.end());
  const unsigned long time = std::stoul(line);

  std::getline(file, line);
  line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char c) { return !static_cast<bool>(std::isdigit(c)); }), line.end());
  const unsigned long distance = std::stoul(line);

  const unsigned long root1 = static_cast<unsigned long>((static_cast<double>(time) - std::sqrt(time * time - 4 * distance)) / 2.0);
  const unsigned long root2 = static_cast<unsigned long>((static_cast<double>(time) + std::sqrt(time * time - 4 * distance)) / 2.0);
  return root2 - root1 - static_cast<unsigned long>(root2 * (time - root2) == distance);
}
