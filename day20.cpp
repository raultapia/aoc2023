#include <algorithm>
#include <array>
#include <fstream>
#include <numeric>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Module {
  std::string name;
  char type;
  std::vector<std::string> out;
  std::unordered_map<std::string, bool> state;
  bool isEnabled() {
    const bool ret = std::all_of(state.begin(), state.end(), [](const std::pair<std::string, bool> &x) { return x.second; });
    return type == '%' ? ret : !ret;
  }
} __attribute__((aligned(128)));

struct Pulse {
  std::string in;
  std::string out;
  bool state;
} __attribute__((aligned(128)));

inline void split(const std::string &s, std::string &r1, std::vector<std::string> &r2) {
  std::string r;
  std::istringstream iss(s);
  iss >> r1;
  iss.ignore(3);
  r2.clear();
  while(iss >> r) {
    if(r.back() == ',') {
      r.pop_back();
    }
    r2.emplace_back(r);
  }
}

unsigned long day20a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::string name;
  std::vector<std::string> out;
  std::unordered_map<std::string, Module> modules;
  std::vector<std::string> init;

  while(std::getline(file, line)) {
    split(line, name, out);

    if(name == "broadcaster") {
      init = out;
    } else {
      modules.insert({name.substr(1), {name.substr(1), name.front(), out, {}}});
    }
  }

  for(std::pair<const std::string, Module> &p : modules) {
    if(p.second.type == '%') {
      p.second.state.insert({"", false});
    }
    for(const std::string &o : p.second.out) {
      if(modules.find(o) != modules.end() && modules.at(o).type == '&') {
        modules.at(o).state.insert({p.first, false});
      }
    }
  }

  std::array<unsigned long, 2> cnt{1000, 0};
  for(int k = 0; k < 1000; k++) {
    std::queue<Pulse> pulses;
    for(const std::string &i : init) {
      pulses.push({"broadcaster", i, false});
    }

    while(!pulses.empty()) {
      const Pulse p = pulses.front();
      pulses.pop();
      cnt.at(static_cast<unsigned long>(p.state))++;

      if(modules.find(p.out) == modules.end()) {
        continue;
      }

      Module &m = modules.at(p.out);
      if(m.type == '&') {
        m.state.at(p.in) = p.state;
      } else if(!p.state) {
        m.state.at("") = !m.state.at("");
      } else {
        continue;
      }
      for(const std::string &o : m.out) {
        pulses.push({m.name, o, m.isEnabled()});
      }
    }
  }

  return cnt[0] * cnt[1];
}

unsigned long day20b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::string name;
  std::vector<std::string> out;
  std::unordered_map<std::string, Module> modules;
  std::vector<std::string> init;

  std::string rx_trig;
  while(std::getline(file, line)) {
    split(line, name, out);

    if(name == "broadcaster") {
      init = out;
    } else {
      modules.insert({name.substr(1), {name.substr(1), name.front(), out, {}}});
      if(std::find(out.begin(), out.end(), "rx") != out.end()) {
        rx_trig = name.substr(1);
      }
    }
  }

  std::unordered_map<std::string, unsigned long> rx_cnt;
  for(std::pair<const std::string, Module> &p : modules) {
    if(p.second.type == '%') {
      p.second.state.insert({"", false});
    }
    for(const std::string &o : p.second.out) {
      if(modules.find(o) != modules.end() && modules.at(o).type == '&') {
        modules.at(o).state.insert({p.first, false});
      }
    }
    if(std::find(p.second.out.begin(), p.second.out.end(), rx_trig) != p.second.out.end()) {
      rx_cnt.insert({p.first, 0UL});
    }
  }

  std::unordered_map<std::string, unsigned long> repeat;
  unsigned long cnt = 0;
  while(static_cast<bool>(++cnt)) {
    std::queue<Pulse> pulses;
    for(const std::string &i : init) {
      pulses.push({"broadcaster", i, false});
    }

    while(!pulses.empty()) {
      const Pulse p = pulses.front();
      pulses.pop();

      if(modules.find(p.out) == modules.end()) {
        continue;
      }

      Module &m = modules.at(p.out);
      if(m.name == rx_trig && p.state) {
        rx_cnt.at(p.in)++;
        repeat.insert({p.in, cnt});
        if(std::all_of(rx_cnt.begin(), rx_cnt.end(), [](const std::pair<std::string, unsigned long> &x) { return x.second; })) {
          return std::accumulate(repeat.begin(), repeat.end(), 1UL, [](unsigned long x, const std::pair<std::string, unsigned long> &r) { return x * static_cast<unsigned long>(r.second / std::gcd(x, r.second)); });
        }
      }

      if(m.type == '&') {
        m.state.at(p.in) = p.state;
      } else if(!p.state) {
        m.state.at("") = !m.state.at("");
      } else {
        continue;
      }
      for(const std::string &o : m.out) {
        pulses.push({m.name, o, m.isEnabled()});
      }
    }
  }
  return 0;
}
