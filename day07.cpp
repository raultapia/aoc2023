#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

enum : std::uint8_t { Value,
                      Type,
                      Bid };

int day07a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::tuple<unsigned long, float, int>> hands;

  while(std::getline(file, line)) {
    std::istringstream iss(line);
    std::string card;
    std::string bid;
    float type = 0;
    iss >> card;
    iss >> bid;
    std::replace(card.begin(), card.end(), 'T', static_cast<char>('9' + 1));
    std::replace(card.begin(), card.end(), 'J', static_cast<char>('9' + 2));
    std::replace(card.begin(), card.end(), 'Q', static_cast<char>('9' + 3));
    std::replace(card.begin(), card.end(), 'K', static_cast<char>('9' + 4));
    std::replace(card.begin(), card.end(), 'A', static_cast<char>('9' + 5));

    unsigned long value = 0;
    std::array<int, 13> count{0};
    for(std::size_t i = 0; i < card.size(); i++) {
      count.at(card[i] - '2')++;
      value += static_cast<unsigned long>(std::pow(100, card.size() - i - 1) * card[i]);
    }
    if(std::find(count.begin(), count.end(), 5) != count.end()) {
      type = 5;
    } else if(std::find(count.begin(), count.end(), 4) != count.end()) {
      type = 4;
    } else if(std::find(count.begin(), count.end(), 3) != count.end()) {
      type = std::find(count.begin(), count.end(), 2) != count.end() ? 3.2 : 3;
    } else if(std::find(count.begin(), count.end(), 2) != count.end()) {
      type = (std::find(count.begin(), count.end(), 2) < (std::find(count.rbegin(), count.rend(), 2) + 1).base()) ? 2.2 : 2;
    }

    hands.emplace_back(value, type, std::stoi(bid));
  }

  std::sort(hands.begin(), hands.end(), [](const std::tuple<unsigned long, float, int> &a, const std::tuple<unsigned long, float, int> &b) {
    return (std::get<Type>(a) != std::get<Type>(b)) ? std::get<Type>(a) < std::get<Type>(b) : std::get<Value>(a) < std::get<Value>(b);
  });

  int result = 0;
  for(std::size_t i = 0; i < hands.size(); i++) {
    result += static_cast<int>(i + 1) * std::get<Bid>(hands[i]);
  }

  return result;
}

unsigned long day07b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::vector<std::tuple<unsigned long, float, int>> hands;

  while(std::getline(file, line)) {
    std::istringstream iss(line);
    std::string card;
    std::string bid;
    float type = 0;
    iss >> card;
    iss >> bid;
    std::replace(card.begin(), card.end(), 'T', static_cast<char>('9' + 1));
    std::replace(card.begin(), card.end(), 'J', static_cast<char>('1'));
    std::replace(card.begin(), card.end(), 'Q', static_cast<char>('9' + 3));
    std::replace(card.begin(), card.end(), 'K', static_cast<char>('9' + 4));
    std::replace(card.begin(), card.end(), 'A', static_cast<char>('9' + 5));

    unsigned long value = 0;
    std::array<int, 14> count{0};
    for(std::size_t i = 0; i < card.size(); i++) {
      count.at(card[i] - '1')++;
      value += static_cast<unsigned long>(std::pow(100, card.size() - i - 1) * card[i]);
    }
    std::array<int, 6> n = {
        1,
        static_cast<int>(std::count(count.begin() + 1, count.end(), 1)),
        static_cast<int>(std::count(count.begin() + 1, count.end(), 2)),
        static_cast<int>(std::count(count.begin() + 1, count.end(), 3)),
        static_cast<int>(std::count(count.begin() + 1, count.end(), 4)),
        static_cast<int>(std::count(count.begin() + 1, count.end(), 5))};

    for(int i = 0; i < 6; i++) {
      if(n.at(5 - i) >= 1 && count[0] >= i) {
        type = 5;
        break;
      }
      if(type < 4 && i < 5 && n.at(4 - i) >= 1 && count[0] >= i) {
        type = 4;
      } else if(type < 3 && i < 4 && n.at(3 - i) >= 1 && count[0] >= i) {
        type = (3 * n[3] + 2 * n[2] + count[0] == 5) ? 3.2 : 3;
      } else if(type < 2 && i < 3 && n.at(2 - i) >= 1 && count[0] >= i) {
        type = (2 * n[2] + count[0] == 4) ? 2.2 : 2;
      }
    }

    hands.emplace_back(value, type, std::stoi(bid));
  }

  std::sort(hands.begin(), hands.end(), [](const std::tuple<unsigned long, float, int> &a, const std::tuple<unsigned long, float, int> &b) {
    return (std::get<Type>(a) != std::get<Type>(b)) ? std::get<Type>(a) < std::get<Type>(b) : std::get<Value>(a) < std::get<Value>(b);
  });

  int result = 0;
  for(std::size_t i = 0; i < hands.size(); i++) {
    result += static_cast<int>(i + 1) * std::get<Bid>(hands[i]);
  }

  return result;
}
