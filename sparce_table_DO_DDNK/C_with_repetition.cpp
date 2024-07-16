#include <algorithm>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

using Pair = std::pair<int, int>;

int MyLog(int number) {
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

std::vector<Pair> FillVector(int amount) {
  std::vector<Pair> segments;
  for (int i = 0; i < amount; ++i) {
    int left = 0;
    int right = 0;
    std::cin >> left;
    std::cin >> right;
    segments.push_back(std::make_pair(left, right));
  }
  return segments;
}

std::vector<Pair> RemoveRepited(std::vector<Pair>& segments) {
  std::vector<Pair> segments_amount_right_coord;
  segments_amount_right_coord.push_back(std::make_pair(1, segments[0].second));
  for (size_t i = 1; i < segments.size(); ++i) {
    if (segments[i] == segments[i - 1]) {
      ++segments_amount_right_coord.back().first;
    } else {
      segments_amount_right_coord.push_back(
          std::make_pair(1, segments[i].second));
    }
  }
  return segments_amount_right_coord;
}

int DecreaseRightCoord(std::vector<Pair>& segments) {
  std::unordered_map<int, int> right_coord;
  std::vector<int> right;
  for (size_t i = 0; i < segments.size(); ++i) {
    right.push_back(segments[i].second);
  }
  std::sort(right.begin(), right.end());
  int cur_index = 1;
  right_coord[right[0]] = 0;
  for (size_t i = 1; i < segments.size(); ++i) {
    if (right[i] != right[i - 1]) {
      right_coord[right[i]] = cur_index;
      ++cur_index;
    }
  }
  for (size_t i = 0; i < segments.size(); ++i) {
    segments[i].second = right_coord[segments[i].second];
  }
  return cur_index;
}

void BuildTree(std::vector<std::vector<int>>& tree,
               const std::vector<Pair>& vec, int in_last_row) {
  for (size_t i = 0; i < vec.size(); ++i) {
    tree[in_last_row - 1 + vec[i].second][0] += vec[i].first;
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
  std::cin.tie(nullptr);
  int amount = 0;
  std::cin >> amount;
  std::vector<Pair> segments = FillVector(amount);
  std::sort(segments.begin(), segments.end(),
            [](Pair first_pair, Pair second_pair) {
              bool flag = false;
              if (first_pair.first < second_pair.first) {
                flag = true;
              } else if (second_pair.first < first_pair.first) {
                flag = false;
              } else if (first_pair.second > second_pair.second) {
                flag = true;
              }
              return flag;
            });
  segments = RemoveRepited(segments);
  int max_right_coord = DecreaseRightCoord(segments);
  int in_last_row = pow(2, MyLog(max_right_coord) + 1);
  std::vector<std::vector<int>> tree(2 * in_last_row - 1,
                                     std::vector<int>(3, 0));
  BuildTree(tree, segments, in_last_row);
  long long answer = 0;
  for (size_t i = 0; i < segments.size(); ++i) {
    Decrease(tree, segments[i].second, segments[i].first, in_last_row);
    answer += segments[i].first * GetSum(tree, 0, segments[i].second);
  }
  std::cout << answer;
}