#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct HashArray2 {
  std::size_t operator()(const std::array<int, 2> &x) const {
    return (static_cast<std::size_t>(x[0]) << 5U) + static_cast<std::size_t>(x[1]);
  }
};

inline bool move(std::array<int, 3> &beam, const int max0, const int max1) {
  switch(beam[2]) {
  case 'u':
    return --beam[0] >= 0;
  case 'd':
    return ++beam[0] < max0;
  case 'l':
    return --beam[1] >= 0;
  case 'r':
    return ++beam[1] < max1;
  default:;
  }
  return false;
}

inline int reflect(const int dir, const bool invert) {
  std::unordered_map<int, int> map = (invert ? std::unordered_map<int, int>{{'u', 'l'}, {'d', 'r'}, {'l', 'u'}, {'r', 'd'}} : std::unordered_map<int, int>{{'u', 'r'}, {'d', 'l'}, {'l', 'd'}, {'r', 'u'}});
  return map[dir];
}

inline bool flat(const int dir, const char tile) {
  return (tile == '|' && (dir == 'l' || dir == 'r')) || (tile == '-' && (dir == 'u' || dir == 'd'));
}

inline int split(int &dir) {
  std::unordered_map<int, std::array<int, 2>> map{{'l', {'u', 'd'}}, {'r', {'u', 'd'}}, {'u', {'l', 'r'}}, {'d', {'l', 'r'}}};
  return map[std::exchange(dir, map[dir][1])][0];
}

inline unsigned long run(const std::vector<std::string> &grid, const std::array<int, 3> start) {
  const int maxr = static_cast<int>(grid.size());
  const int maxc = static_cast<int>(grid[0].size());

  std::unordered_set<std::array<int, 2>, HashArray2> energized;
  std::unordered_set<std::array<int, 2>, HashArray2> splitters;
  std::vector<std::array<int, 3>> beams{start};

  while(!beams.empty()) {
    for(std::size_t k = 0; k < beams.size();) {
      energized.insert({beams[k][0], beams[k][1]});
      const char g = grid.at(beams[k][0]).at(beams[k][1]);
      switch(g) {
      case '/':
      case '\\':
        beams[k][2] = reflect(beams[k][2], g == '\\');
        break;
      case '|':
      case '-':
        if(flat(beams[k][2], g)) {
          if(splitters.insert({beams[k][0], beams[k][1]}).second) {
            beams.emplace_back(std::array{beams[k][0], beams[k][1], split(beams[k][2])});
          } else {
            beams.erase(beams.begin() + static_cast<long>(k));
            continue;
          }
        }
      default:;
      }
      if(!move(beams[k], maxr, maxc)) {
        beams.erase(beams.begin() + static_cast<long>(k));
      } else {
        k++;
      }
    }
  }
  return energized.size();
}

unsigned long day16a(const std::string &filename) {
  std::ifstream file(filename);
  const std::vector<std::string> grid{std::istream_iterator<std::string>{file}, std::istream_iterator<std::string>{}};
  return run(grid, std::array<int, 3>{0, 0, 'r'});
}

unsigned long day16b(const std::string &filename) {
  std::ifstream file(filename);
  std::vector<std::string> grid{std::istream_iterator<std::string>{file}, std::istream_iterator<std::string>{}};
  const int m = static_cast<int>(grid[0].size());
  const int n = static_cast<int>(grid.size());
  unsigned long ret = 0;
  for(int i = 0; i < std::max(m, n); i++) {
    if(i < n) {
      ret = std::max({ret, run(grid, std::array<int, 3>{0, i, 'd'}), run(grid, std::array<int, 3>{n - 1, i, 'u'})});
    }
    if(i < m) {
      ret = std::max({ret, run(grid, std::array<int, 3>{i, 0, 'r'}), run(grid, std::array<int, 3>{i, m - 1, 'l'})});
    }
  }
  return ret;
}
