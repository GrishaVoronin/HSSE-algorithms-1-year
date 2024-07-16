#include <cmath>
#include <iostream>
#include <vector>

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

void BuildTree(std::vector<std::vector<int>>& tree,
               const std::vector<int>& last_row, int in_last_row) {
  for (int i = 0; i < in_last_row; ++i) {
    if (i % 2 == 1) {
      tree[in_last_row - 1 + i][0] = -1 * last_row[i];
    } else {
      tree[in_last_row - 1 + i][0] = last_row[i];
    }
    tree[in_last_row - 1 + i][1] = i;
    tree[in_last_row - 1 + i][2] = i;
  }
  for (int i = in_last_row - 2; i >= 0; --i) {
    tree[i][0] = tree[2 * i + 1][0] + tree[2 * i + 2][0];
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

void Change(std::vector<std::vector<int>>& tree, int position, int value,
            int in_last_row) {
  if (position % 2 == 0) {
    value *= -1;
  }
  position = in_last_row + position - 2;
  tree[position][0] = value;
  Update(tree, (position - 1) / 2);
}

void InputHandler(std::vector<std::vector<int>>& tree, int in_last_row,
                  int questions) {
  for (int i = 0; i < questions; ++i) {
    int command = 0;
    std::cin >> command;
    if (command == 1) {
      int left = 0;
      int right = 0;
      std::cin >> left >> right;
      --left;
      --right;
      if (left % 2 == 0) {
        std::cout << GetSum(tree, left, right) << "\n";

      } else {
        std::cout << -1 * GetSum(tree, left, right) << "\n";
      }
    } else {
      int position = 0;
      int value = 0;
      std::cin >> position >> value;
      Change(tree, position, value, in_last_row);
    }
  }
}

int main() {
  int amout = 0;
  std::cin >> amout;
  int in_last_row = pow(2, MyLog(amout));
  std::vector<std::vector<int>> tree(2 * in_last_row - 1,
                                     std::vector<int>(3, 0));
  std::vector<int> last_row(in_last_row, 0);
  for (int i = 0; i < amout; ++i) {
    std::cin >> last_row[i];
  }
  BuildTree(tree, last_row, in_last_row);
  int questions = 0;
  std::cin >> questions;
  InputHandler(tree, in_last_row, questions);
}