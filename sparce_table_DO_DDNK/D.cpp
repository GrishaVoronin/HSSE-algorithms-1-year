#include <cmath>
#include <iostream>
#include <vector>

namespace Constants {
const int kMaxElement = 1000000001;
const int kMaxElemIndex = -1;
}  // namespace Constants

int MyLogUp(int number) {
  int add = 0;
  int answer = 0;
  while (number > 1) {
    if (number % 2 == 1) {
      add = 1;
    }
    number /= 2;
    ++answer;
  }
  return answer + add;
}

int MyLogDown(int number) {
  int answer = 0;
  while (number > 1) {
    number /= 2;
    ++answer;
  }
  return answer;
}

bool Compare(std::pair<int, int> first_pair, std::pair<int, int> second_pair) {
  return first_pair.first < second_pair.first;
}

void QuickSort(std::vector<std::pair<int, int>>& vec, int start, int finish) {
  if (start >= finish) {
    return;
  }
  std::pair<int, int> pivot = vec[start];
  int left = start;
  int right = finish;

  while (left < right) {
    while (left < finish && Compare(vec[left], pivot)) {
      left++;
    }
    while (right > start && Compare(pivot, vec[right])) {
      right--;
    }
    if (left <= right) {
      std::swap(vec[left], vec[right]);
      left++;
      right--;
    }
  }
  QuickSort(vec, start, right);
  QuickSort(vec, left, finish);
}

std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>
BuildSparseTable(std::vector<int>& numbers, int len) {
  std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>
      sparce_table(
          MyLogUp(len),
          std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>(
              len));
  for (int i = 0; i < len; ++i) {
    sparce_table[0][i].first.first = numbers[i];
    sparce_table[0][i].first.second = i;
    sparce_table[0][i].second.first = Constants::kMaxElement;
    sparce_table[0][i].second.second = Constants::kMaxElemIndex;
  }
  for (int i = 1; i < MyLogUp(len); ++i) {
    for (int j = 0; j + pow(2, i) <= len; ++j) {
      std::vector<std::pair<int, int>> first_four;
      first_four.push_back(sparce_table[i - 1][j].first);
      first_four.push_back(sparce_table[i - 1][j].second);
      first_four.push_back(sparce_table[i - 1][j + pow(2, i - 1)].first);
      first_four.push_back(sparce_table[i - 1][j + pow(2, i - 1)].second);
      QuickSort(first_four, 0, 3);
      sparce_table[i][j] = std::make_pair(first_four[0], first_four[1]);
    }
  }
  return sparce_table;
}

void QuestionsHandler(
    std::vector<
        std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>
        sparse_table,
    int questions) {
  for (int i = 0; i < questions; ++i) {
    int left = 0;
    int right = 0;
    std::cin >> left;
    std::cin >> right;
    --left;
    --right;
    int max_deg = MyLogDown(right - left + 1);
    std::vector<std::pair<int, int>> first_four;
    first_four.push_back(sparse_table[max_deg][left].first);
    first_four.push_back(sparse_table[max_deg][left].second);
    first_four.push_back(
        sparse_table[max_deg][right - pow(2, max_deg) + 1].first);
    first_four.push_back(
        sparse_table[max_deg][right - pow(2, max_deg) + 1].second);
    QuickSort(first_four, 0, 3);
    if (first_four[0].second != first_four[1].second) {
      std::cout << first_four[1].first << "\n";
    } else {
      std::cout << first_four[2].first << "\n";
    }
  }
}

int main() {
  int amount = 0;
  std::cin >> amount;
  int questions = 0;
  std::cin >> questions;
  std::vector<int> numbers(amount, 0);
  for (int i = 0; i < amount; ++i) {
    std::cin >> numbers[i];
  }
  std::vector<std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>
      sparse_table = BuildSparseTable(numbers, amount);
  QuestionsHandler(sparse_table, questions);
}