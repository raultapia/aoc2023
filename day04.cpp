#include <algorithm>
#include <fstream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

int day04a(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  int result = 0;
  int k = 0;
  std::vector<int> winning;
  std::vector<int> yours;
  int n_winning = -1;
  int n_yours = -1;

  do {
    file >> line;
  } while(line.back() != ':');
  do {
    file >> line;
    n_winning++;
  } while(line.back() != '|');
  do {
    file >> line;
    n_yours++;
  } while(line.front() != 'C');
  file.seekg(0, std::ios::beg);

  while(file >> line) {
    if(k > 1 && k < n_winning + 2) {
      winning.push_back(std::stoi(line));
    } else if(k > n_winning + 2 && k < n_winning + n_yours + 3) {
      yours.push_back(std::stoi(line));
    }
    if(k++ > n_winning + n_yours + 1) {
      std::vector<int> u;
      std::sort(winning.begin(), winning.end());
      std::sort(yours.begin(), yours.end());
      std::set_intersection(winning.begin(), winning.end(), yours.begin(), yours.end(), std::back_inserter(u));
      winning.clear();
      yours.clear();
      if(!u.empty()) {
        result += static_cast<int>(1U << (u.size() - 1));
      }
      k = 0;
    }
  }

  return result;
}

int day04b(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  int k = 0;
  int card = 0;
  std::vector<int> winning;
  std::vector<int> yours;
  int n_winning = -1;
  int n_yours = -1;

  do {
    file >> line;
  } while(line.back() != ':');
  do {
    file >> line;
    n_winning++;
  } while(line.back() != '|');
  do {
    file >> line;
    n_yours++;
  } while(line.front() != 'C');
  file.seekg(0, std::ios::beg);

  while(file >> line) {
    if(line.front() == 'C') {
      card++;
    }
  }
  file.clear();
  file.seekg(0, std::ios::beg);

  std::vector<int> instances(card, 1);
  card = 1;
  while(file >> line) {
    if(k > 1 && k < n_winning + 2) {
      winning.push_back(std::stoi(line));
    } else if(k > n_winning + 2 && k < n_winning + n_yours + 3) {
      yours.push_back(std::stoi(line));
    }
    if(k++ > n_winning + n_yours + 1) {
      std::vector<int> u;
      std::sort(winning.begin(), winning.end());
      std::sort(yours.begin(), yours.end());
      std::set_intersection(winning.begin(), winning.end(), yours.begin(), yours.end(), std::back_inserter(u));
      winning.clear();
      yours.clear();
      for(int m = 0; m < static_cast<int>(u.size()); m++) {
        instances[card + m] += instances[card - 1];
      }
      k = 0;
      card++;
    }
  }

  return std::accumulate(instances.begin(), instances.end(), 0);
}
