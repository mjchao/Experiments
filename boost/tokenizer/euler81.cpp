#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

constexpr int INF = 999999;
constexpr int N = 80;

std::vector<std::vector<int>> matrix(N, std::vector<int>(N, -1));
std::vector<std::vector<int>> cache(N, std::vector<int>(N, INF));

int MinSum(int row, int col) {
  if (row == N - 1 && col == N - 1) {
    return matrix[row][col];
  }

  if (cache[row][col] != INF) {
    return cache[row][col];
  }

  int min_sum_down = INF;
  if (row < N - 1) {
    min_sum_down = matrix[row][col] + MinSum(row + 1, col);
  }

  int min_sum_right = INF;
  if (col < N - 1) {
    min_sum_right = matrix[row][col] + MinSum(row, col + 1);
  }

  cache[row][col] = std::min(min_sum_down, min_sum_right);
  return cache[row][col];
}

int main() {
  std::ifstream fin("euler81.txt");

  std::string file_line;
  int row = 0;
  while (getline(fin, file_line)) {
    boost::char_separator<char> sep(",");
    boost::tokenizer< boost::char_separator<char> > tok(file_line, sep);
    int col = 0;
    for (const std::string& s : tok) {
      matrix[row][col] = boost::lexical_cast<int>(s);
      ++col;
    }
    ++row;
  }

  int answer = MinSum(0, 0);
  std::cout << answer << std::endl;
  return 0;
}
