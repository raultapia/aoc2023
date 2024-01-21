#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <initializer_list>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

inline bool execute(const std::unordered_map<std::string, std::vector<std::string>> &workflows, const std::unordered_map<char, int> &ratings) {
  std::string current = "in";
  while(true) {
    if(current.size() == 1) {
      return current.front() == 'A';
    }
    for(const std::string &cmd : workflows.at(current)) {
      if(cmd.find(':') != std::string::npos) {
        std::istringstream iss2(cmd);
        iss2.ignore(2);
        int n = 0;
        iss2 >> n;
        iss2.ignore(1);
        if((cmd[1] == '>' && ratings.at(cmd[0]) > n) || (cmd[1] == '<' && ratings.at(cmd[0]) < n)) {
          iss2 >> current;
          break;
        }
      } else {
        current = cmd;
        break;
      }
    }
  }
}

unsigned long count(const std::unordered_map<std::string, std::vector<std::string>> &workflows, std::unordered_map<char, std::array<int, 2>> &ratings, std::string current = "in") {
  if(current.front() == 'R') {
    return 0;
  }
  if(current.front() == 'A') {
    return std::accumulate(ratings.begin(), ratings.end(), 1UL, [](unsigned long acc, const std::pair<char, std::array<int, 2>> &r) { return acc * (r.second[1] - r.second[0] + 1); });
  }

  unsigned long ret = 0;
  for(const std::string &cmd : workflows.at(current)) {
    if(cmd.find(':') != std::string::npos) {
      std::istringstream iss2(cmd);
      std::unordered_map<char, std::array<int, 2>> new_ratings(ratings);
      std::string new_current;
      iss2.ignore(2);
      int n = 0;
      iss2 >> n;
      iss2.ignore(1);
      iss2 >> new_current;
      if(cmd[1] == '>') {
        new_ratings[cmd[0]][0] = std::max(new_ratings[cmd[0]][0], n + 1);
        ratings[cmd[0]][1] = new_ratings[cmd[0]][0] - 1;
      } else {
        new_ratings[cmd[0]][1] = std::min(new_ratings[cmd[0]][1], n - 1);
        ratings[cmd[0]][0] = new_ratings[cmd[0]][1] + 1;
      }
      if(new_ratings[cmd[0]][0] <= new_ratings[cmd[0]][1]) {
        ret += count(workflows, new_ratings, new_current);
      }
    } else {
      ret += count(workflows, ratings, cmd);
    }
  }
  return ret;
}

unsigned long day19a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::unordered_map<std::string, std::vector<std::string>> workflows;
  std::unordered_map<char, int> ratings;
  unsigned long result = 0;

  while(file >> line) {
    if(line.front() != '{') {
      const std::size_t pos = line.find('{');
      const std::string key = line.substr(0, pos);
      const std::string value = line.substr(pos + 1, line.find('}') - pos - 1);
      std::istringstream iss(value);
      std::string s;
      while(std::getline(iss, s, ',')) {
        workflows[key].emplace_back(s);
      }
    } else {
      std::istringstream iss(line);
      for(const char c : {'x', 'm', 'a', 's'}) {
        iss.ignore(3);
        iss >> ratings[c];
      }
      if(execute(workflows, ratings)) {
        result += ratings['x'] + ratings['m'] + ratings['a'] + ratings['s'];
      }
    }
  }

  return result;
}

unsigned long day19b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::unordered_map<std::string, std::vector<std::string>> workflows;
  std::unordered_map<char, std::array<int, 2>> ratings;

  while(file >> line) {
    if(line.front() != '{') {
      const std::size_t pos = line.find('{');
      const std::string key = line.substr(0, pos);
      const std::string value = line.substr(pos + 1, line.find('}') - pos - 1);
      std::istringstream iss(value);
      std::string s;
      while(std::getline(iss, s, ',')) {
        workflows[key].emplace_back(s);
      }
    }
  }

  for(const char c : {'x', 'm', 'a', 's'}) {
    ratings[c] = {1, 4000};
  }
  return count(workflows, ratings);
}
