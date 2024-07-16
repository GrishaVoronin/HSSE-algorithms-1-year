#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace Constants {
const int kUsers = 1e5;
const int kInLastRow = 65536;
const int kNumsAfterPoint = 6;
}  // namespace Constants

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

void BuildTree(std::vector<std::vector<int>>& tree) {
  for (int i = 0; i < Constants::kInLastRow; ++i) {
    tree[Constants::kInLastRow - 1 + i][1] = i;
    tree[Constants::kInLastRow - 1 + i][2] = i;
  }
  for (int i = Constants::kInLastRow - 2; i >= 0; --i) {
    tree[i][1] = tree[2 * i + 1][1];
    tree[i][2] = tree[2 * i + 2][2];
  }
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

void Update(std::vector<std::vector<int>>& tree, int position) {
  if (position >= 0) {
    tree[position][0] = tree[2 * position + 1][0] + tree[2 * position + 2][0];
    if (position > 0) {
      Update(tree, (position - 1) / 2);
    }
  }
}

void Change(std::vector<std::vector<int>>& tree, int position, int value) {
  position = Constants::kInLastRow + position - 2;
  tree[position][0] += value;
  Update(tree, (position - 1) / 2);
}

void InputHandler(std::vector<std::vector<int>>& tree, std::vector<int>& users,
                  int questions) {
  double cur_users = 0;
  for (int i = 0; i < questions; ++i) {
    std::string command;
    std::cin >> command;
    if (command == "RUN") {
      int user = 0;
      std::cin >> user;
      int seconds = 0;
      std::cin >> seconds;
      if (users[user] == 0) {
        ++cur_users;
        Change(tree, seconds, 1);
      } else {
        Change(tree, users[user], -1);
        Change(tree, seconds, 1);
      }
      users[user] = seconds;
    } else {
      int user = 0;
      std::cin >> user;
      if (users[user] == 0) {
        std::cout << 0 << "\n";
      } else if (cur_users == 1) {
        std::cout << 1 << "\n";
      } else {
        std::cout << std::setprecision(Constants::kNumsAfterPoint)
                  << GetSum(tree, 0, users[user] - 2) / (cur_users - 1) << "\n";
      }
    }
  }
}

int main() {
  std::cin.tie(nullptr);
  int questions = 0;
  std::cin >> questions;
  std::vector<std::vector<int>> tree(2 * Constants::kInLastRow - 1,
                                     std::vector<int>(3, 0));
  BuildTree(tree);
  std::vector<int> users(Constants::kUsers, 0);
  InputHandler(tree, users, questions);
}