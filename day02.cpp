#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <utility>

int day02a(const std::string &filename) {
  std::ifstream file(filename);
  std::string key;
  std::string value;
  std::map<const std::string, int> cubes{{"red", 0}, {"green", 0}, {"blue", 0}};
  std::map<const std::string, const int> max{{"red", 12}, {"green", 13}, {"blue", 14}};
  int result = 0;
  int game = 0;
  bool possible = true;

  while(file >> value) {
    file >> key;
    if(value == "Game") {
      if(possible) {
        result += game;
      }
      game = std::stoi(key);
      possible = true;
    } else {
      if(key.back() == ',') {
        key.pop_back();
        cubes[key] = std::stoi(value);
        continue;
      }
      if(key.back() == ';') {
        key.pop_back();
      }
      cubes[key] = std::stoi(value);
      for(std::pair<const std::string, int> &x : cubes) {
        possible = possible && (x.second <= max[x.first]);
        x.second = 0;
      }
    }
  }

  return possible ? result + game : result;
}

int day02b(const std::string &filename) {
  std::ifstream file(filename);
  std::string key;
  std::string value;
  std::map<const std::string, int> cubes{{"red", 0}, {"green", 0}, {"blue", 0}};
  int result = 0;

  while(file >> value) {
    file >> key;
    if(value != "Game") {
      if(key.back() == ',' || key.back() == ';') {
        key.pop_back();
        cubes[key] = std::max(cubes[key], std::stoi(value));
      } else {
        cubes[key] = std::max(cubes[key], std::stoi(value));
        int power = 1;
        for(std::pair<const std::string, int> &x : cubes) {
          power *= x.second;
          x.second = 0;
        }
        result += power;
      }
    }
  }

  return result;
}
