#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

struct Coordinate {
  int row;
  int col;
  [[nodiscard]] bool operator==(const Coordinate &c) const {
    return row == c.row && col == c.col;
  }
  [[nodiscard]] bool operator!=(const Coordinate &c) const {
    return row != c.row || col != c.col;
  }
  [[nodiscard]] Coordinate operator+(const Coordinate &c) const {
    return {row + c.row, col + c.col};
  }
  [[nodiscard]] bool outside(const std::vector<std::vector<int>> &m) const {
    const int maxr = static_cast<int>(m.size());
    const int maxc = static_cast<int>(m[0].size());
    return row < 0 || row >= maxr || col < 0 || col >= maxc;
  }
  [[nodiscard]] Coordinate opposite() const {
    return {-row, -col};
  }
} __attribute__((aligned(8)));

class Node {
public:
  Node(const Coordinate p, const Coordinate d, const unsigned long c = 0, const unsigned int b = 1) : pos{p}, dir{d}, cost{c}, blocks{b} {}
  Coordinate pos;
  Coordinate dir;
  unsigned long cost;
  unsigned int blocks;
  [[nodiscard]] bool operator==(const Node &n) const {
    return pos == n.pos && dir == n.dir;
  }
  [[nodiscard]] bool operator<(const Node &n) const {
    return cost > n.cost;
  }
};

struct NodeHash {
  std::size_t operator()(const Node &node) const {
    return static_cast<std::size_t>(node.pos.row) ^ (static_cast<std::size_t>(node.pos.col) << 5U) ^ (static_cast<std::size_t>(node.dir.row) << 10U) ^ (static_cast<std::size_t>(node.dir.col) << 15U) ^ (static_cast<std::size_t>(node.blocks) << 20U);
  }
};

unsigned long solve(const std::vector<std::vector<int>> &map, const unsigned int min, const unsigned int max) {
  const Coordinate target = {static_cast<int>(map.size() - 1), static_cast<int>(map[0].size() - 1)};
  std::unordered_set<Node, NodeHash> visited;
  std::priority_queue<Node> q;
  q.emplace(Coordinate{0, 0}, Coordinate{0, 0});
  while(!q.empty()) {
    const Node node = q.top();
    q.pop();

    if(node.pos == target) {
      return node.cost;
    }
    if(visited.find(node) != visited.end()) {
      continue;
    }
    visited.insert(node);

    for(const Coordinate &d : std::vector<Coordinate>{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
      const Coordinate c = node.pos + d;
      if(node.dir == d.opposite() || c.outside(map) || (node.dir == d && node.blocks == max) || (node.dir != d && node.blocks < min && node.dir != Coordinate{0, 0})) {
        continue;
      }
      if(node.dir == d || node.dir == Coordinate{0, 0}) {
        q.emplace(c, d, node.cost + map[node.pos.row + d.row][node.pos.col + d.col], node.blocks + 1);
      } else {
        q.emplace(c, d, node.cost + map[node.pos.row + d.row][node.pos.col + d.col]);
      }
    }
  }
  return 0;
}

unsigned long day17a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::vector<int>> map;
  while(file >> line) {
    std::transform(line.begin(), line.end(), std::back_inserter(map.emplace_back()), [](char c) { return static_cast<int>(c - '0'); });
  }
  return solve(map, 0, 3);
}

unsigned long day17b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::vector<int>> map;
  while(file >> line) {
    std::transform(line.begin(), line.end(), std::back_inserter(map.emplace_back()), [](char c) { return static_cast<int>(c - '0'); });
  }
  return solve(map, 4, 10);
}
