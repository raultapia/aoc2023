#include <array>
#include <cstddef>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Hash {
  std::size_t operator()(const std::array<int, 2> &x) const {
    return (static_cast<std::size_t>(x[0]) << 5U) + static_cast<std::size_t>(x[1]);
  }
};

int day10a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> pipes;
  std::array<int, 6> p{0};
  int result = 0;

  while(file >> line) {
    pipes.emplace_back(line);
    if(line.find('S') != std::string::npos) {
      p[3] = p[0] = static_cast<int>(pipes.size() - 1);
      p[4] = p[1] = static_cast<int>(line.find('S'));
    }
  }

  if(pipes.at(p[0] + 1).at(p[1]) == 'J' || pipes.at(p[0] + 1).at(p[1]) == 'L') {
    p[2] = (pipes.at(++p[0]).at(p[1]) == 'L') ? 1 : -1;
  } else if(pipes.at(p[0]).at(p[1] + 1) == '7' || pipes.at(p[0]).at(p[1] + 1) == '-' || pipes.at(p[0]).at(p[1] + 1) == 'J') {
    p[2] = (pipes.at(p[0]).at(++p[1]) == '7') ? -1 : 1;
  } else {
    p[2] = (pipes.at(p[0]).at(--p[1]) == 'F') ? 1 : -1;
  }
  if(pipes.at(p[3] - 1).at(p[4]) == '7' || pipes.at(p[3] - 1).at(p[4]) == '|' || pipes.at(p[3] - 1).at(p[4]) == 'F') {
    p[5] = (pipes.at(--p[3]).at(p[4]) == 'F') ? -1 : 1;
  } else if(pipes.at(p[3] + 1).at(p[4]) == 'J' || pipes.at(p[3] + 1).at(p[4]) == '|' || pipes.at(p[3] + 1).at(p[4]) == 'L') {
    p[5] = (pipes.at(++p[3]).at(p[4]) == 'L') ? 1 : -1;
  } else if(pipes.at(p[3]).at(p[4] + 1) == '7' || pipes.at(p[3]).at(p[4] + 1) == '-' || pipes.at(p[3]).at(p[4] + 1) == 'J') {
    p[5] = (pipes.at(p[3]).at(++p[4]) == '7') ? -1 : 1;
  }

  while(result++, p[0] != p[3] || p[1] != p[4]) {
    for(int i = 0; i < 6; i += 3) {
      if(p.at(i + 2) > 0) {
        switch(pipes.at(p.at(i)).at(p.at(i + 1))) {
        case '|':
        case 'J':
          p.at(i + 2) = (pipes.at(--p.at(i)).at(p.at(i + 1)) == 'F') ? -1 : 1;
          break;
        case '-':
        case 'L':
          p.at(i + 2) = (pipes.at(p.at(i)).at(++p.at(i + 1)) == '7') ? -1 : 1;
          break;
        case '7':
          p.at(i + 2) = (pipes.at(p.at(i)).at(--p.at(i + 1)) == 'F') ? 1 : -1;
          break;
        default: // F
          p.at(i + 2) = (pipes.at(++p.at(i)).at(p.at(i + 1)) == 'L') ? 1 : -1;
        }
      } else {
        switch(pipes.at(p.at(i)).at(p.at(i + 1))) {
        case '|':
        case '7':
          p.at(i + 2) = (pipes.at(++p.at(i)).at(p.at(i + 1)) == 'L') ? 1 : -1;
          break;
        case 'J':
        case '-':
          p.at(i + 2) = (pipes.at(p.at(i)).at(--p.at(i + 1)) == 'F') ? 1 : -1;
          break;
        case 'L':
          p.at(i + 2) = (pipes.at(--p.at(i)).at(p.at(i + 1)) == 'F') ? -1 : 1;
          break;
        default: // F
          p.at(i + 2) = (pipes.at(p.at(i)).at(++p.at(i + 1)) == '7') ? -1 : 1;
        }
      }
    }
  }

  return result;
}

int day10b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> pipes;
  std::array<int, 3> p{0};
  std::unordered_set<std::array<int, 2>, Hash> mainloop;
  int result = 0;

  while(file >> line) {
    pipes.emplace_back(line);
    if(line.find('S') != std::string::npos) {
      p[0] = static_cast<int>(pipes.size() - 1);
      p[1] = static_cast<int>(line.find('S'));
    }
  }

  if(pipes.at(p[0] + 1).at(p[1]) == 'J' || pipes.at(p[0] + 1).at(p[1]) == 'L') {
    p[2] = (pipes.at(++p[0]).at(p[1]) == 'L') ? 1 : -1;
  } else if(pipes.at(p[0]).at(p[1] + 1) == '7' || pipes.at(p[0]).at(p[1] + 1) == '-' || pipes.at(p[0]).at(p[1] + 1) == 'J') {
    p[2] = (pipes.at(p[0]).at(++p[1]) == '7') ? -1 : 1;
  } else {
    p[2] = (pipes.at(p[0]).at(--p[1]) == 'F') ? 1 : -1;
  }

  mainloop.emplace(std::array<int, 2>{p[0], p[1]});
  do {
    if(p[2] > 0) {
      switch(pipes.at(p[0]).at(p[1])) {
      case '|':
      case 'J':
        p[2] = (pipes.at(--p[0]).at(p[1]) == 'F') ? -1 : 1;
        break;
      case '-':
      case 'L':
        p[2] = (pipes.at(p[0]).at(++p[1]) == '7') ? -1 : 1;
        break;
      case '7':
        p[2] = (pipes.at(p[0]).at(--p[1]) == 'F') ? 1 : -1;
        break;
      default: // F
        p[2] = (pipes.at(++p[0]).at(p[1]) == 'L') ? 1 : -1;
      }
    } else {
      switch(pipes.at(p[0]).at(p[1])) {
      case '|':
      case '7':
        p[2] = (pipes.at(++p[0]).at(p[1]) == 'L') ? 1 : -1;
        break;
      case 'J':
      case '-':
        p[2] = (pipes.at(p[0]).at(--p[1]) == 'F') ? 1 : -1;
        break;
      case 'L':
        p[2] = (pipes.at(--p[0]).at(p[1]) == 'F') ? -1 : 1;
        break;
      default: // F
        p[2] = (pipes.at(p[0]).at(++p[1]) == '7') ? -1 : 1;
      }
    }
  } while(mainloop.emplace(std::array<int, 2>{p[0], p[1]}), pipes.at(p[0]).at(p[1]) != 'S');

  for(int i = 0; i < static_cast<int>(pipes.size()); i++) {
    for(int j = 0; j < static_cast<int>(pipes.at(i).size()); j++) {
      if(mainloop.find({i, j}) != mainloop.end()) {
        continue;
      }
      int cnt = 0;
      std::array<int, 2> ray{i, j};
      while(++ray[0] < static_cast<int>(pipes.size()) && ++ray[1] < static_cast<int>(pipes.at(i).size())) {
        if(mainloop.find(ray) != mainloop.end() && pipes.at(ray[0]).at(ray[1]) != 'L' && pipes.at(ray[0]).at(ray[1]) != '7') {
          cnt++;
        }
      }
      if(cnt % 2 == 1) {
        result++;
      }
    }
  }

  return result;
}
