#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

bool Compare(std::pair<int, int> first, std::pair<int, int> second) {
  bool flag = false;
  if (first.first < second.first) {
    flag = true;
  } else if (second.first < first.first) {
    flag = false;
  } else if (first.second > second.second) {
    flag = true;
  }
  return flag;
}

void QuickSort(std::vector<int>& vec, int start, int finish) {
  if (start >= finish) {
    return;
  }
  int pivot = vec[start];
  int left = start;
  int right = finish;

  while (left < right) {
    while (left < finish && vec[left] < pivot) {
      left++;
    }
    while (right > start && vec[right] > pivot) {
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

std::vector<std::pair<int, int>> CreateVec(int amount) {
  std::vector<std::pair<int, int>> segments;
  for (int i = 0; i < amount; ++i) {
    int left = 0;
    std::cin >> left;
    int right = 0;
    std::cin >> right;
    segments.push_back(std::make_pair(left, right));
  }
  QuickSort(segments, 0, segments.size() - 1);
  std::vector<std::pair<int, int>> final_vec;
  final_vec.push_back(std::make_pair(1, segments[0].second));
  std::vector<int> right_coord;
  right_coord.push_back(segments[0].second);
  for (int i = 1; i < amount; ++i) {
    if (segments[i] == segments[i - 1]) {
      ++final_vec.back().first;
    } else {
      final_vec.push_back(std::make_pair(1, segments[i].second));
      right_coord.push_back(segments[i].second);
    }
  }
  QuickSort(right_coord, 0, right_coord.size() - 1);
  std::unordered_map<int, int> map;
  for (size_t i = 0; i < right_coord.size(); ++i) {
    map[right_coord[i]] = i;
  }
  for (size_t i = 0; i < final_vec.size(); ++i) {
    final_vec[i].second = map[final_vec[i].second];
  }
  return final_vec;
}

void BuildTree(std::vector<std::vector<int>>& tree,
               const std::vector<std::pair<int, int>>& vec, int in_last_row) {
  for (size_t i = 0; i < vec.size(); ++i) {
    tree[in_last_row - 1 + vec[i].second][0] += vec[i].first;
    tree[in_last_row - 1 + i][1] = i;
    tree[in_last_row - 1 + i][2] = i;
  }
  for (int i = 0; i < in_last_row; ++i) {
    tree[in_last_row - 1 + i][1] = i;
    tree[in_last_row - 1 + i][2] = i;
  }
  for (int i = in_last_row - 2; i >= 0; --i) {
    tree[i][0] = tree[2 * i + 1][0] + tree[2 * i + 2][0];
    tree[i][1] = tree[2 * i + 1][1];
    tree[i][2] = tree[2 * i + 2][2];
  }
}

void Update(std::vector<std::vector<int>>& tree, int position) {
  if (position >= 0) {
    tree[position][0] = tree[2 * position + 1][0] + tree[2 * position + 2][0];
    if (position > 0) {
      Update(tree, (position - 1) / 2);
    }
  }
}

void Decrease(std::vector<std::vector<int>>& tree, int position, int value,
              int in_last_row) {
  position = in_last_row + position - 1;
  tree[position][0] -= value;
  Update(tree, (position - 1) / 2);
}

int GetSum(std::vector<std::vector<int>>& tree, int left, int right,
           int position = 0) {
  if (left <= tree[position][1] && tree[position][2] <= right) {
    return tree[position][0];
  }
  if (tree[position][2] < left || tree[position][1] > right) {
    return 0;
  }
  return GetSum(tree, left, right, 2 * position + 1) +
      GetSum(tree, left, right, 2 * position + 2);
}

int main() {
  int amount = 0;
  std::cin >> amount;
  std::vector<std::pair<int, int>> vec = CreateVec(amount);
  int in_last_row = pow(2, MyLog(vec.size()));
  std::vector<std::vector<int>> tree(2 * in_last_row - 1,
                                     std::vector<int>(3, 0));
  BuildTree(tree, vec, in_last_row);
  int answer = 0;
  for (size_t i = 0; i < vec.size(); ++i) {
    Decrease(tree, vec[i].second, vec[i].first, in_last_row);
    answer += vec[i].first * GetSum(tree, 0, vec[i].second);
  }
  std::cout << answer;
}