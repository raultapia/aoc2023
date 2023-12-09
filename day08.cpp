#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

int day08a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<int> sequence;
  std::unordered_map<std::string, std::array<std::string, 2>> network;
  int result = 0;

  std::getline(file, line);
  std::transform(line.begin(), line.end(), std::back_inserter(sequence), [](char c) { return static_cast<int>(c == 'R'); });
  file.ignore(1);

  while(std::getline(file, line)) {
    network.emplace(line.substr(0, 3), std::array<std::string, 2>{line.substr(7, 3), line.substr(12, 3)});
  }

  std::string node = "AAA";
  while(node != "ZZZ") {
    node = network.at(node).at(sequence[result++ % sequence.size()]);
  }

  return result;
}

unsigned long day08b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<int> sequence;
  std::unordered_map<std::string, std::array<std::string, 2>> network;
  std::vector<std::string> nodes;
  unsigned long result = 0;

  std::getline(file, line);
  std::transform(line.begin(), line.end(), std::back_inserter(sequence), [](char c) { return static_cast<int>(c == 'R'); });
  file.ignore(1);

  while(std::getline(file, line)) {
    network.emplace(line.substr(0, 3), std::array<std::string, 2>{line.substr(7, 3), line.substr(12, 3)});
    if(line[2] == 'A') {
      nodes.emplace_back(line.substr(0, 3));
    }
  }

  std::vector<unsigned long> loops(nodes.size(), 0);
  do {
    for(size_t i = 0; i < nodes.size(); i++) {
      if(static_cast<bool>(loops[i])) {
        continue;
      }
      nodes[i] = network.at(nodes[i]).at(sequence[result % sequence.size()]);
      if(nodes[i].back() == 'Z') {
        loops[i] = result + 1;
      }
    }
    result++;
  } while(std::any_of(loops.begin(), loops.end(), [](unsigned long c) { return !static_cast<bool>(c); }));

  result = 1;
  for(const unsigned long loop : loops) {
    result = std::lcm(result, loop);
  }
  return result;
}
