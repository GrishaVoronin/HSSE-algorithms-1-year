#include <cmath>
#include <iostream>

namespace Constants {
const int kMaxPriority = 1000000000;
}

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

class Treap {
 public:
  Treap() = default;

  ~Treap() { ClearTreap(root_); }

  void Insert(int elem);
  void Delete(int elem);

 private:
  struct Node {
    explicit Node(int key)
        : key(key),
          priority(rand() % Constants::kMaxPriority),
          left_child(nullptr),
          right_child(nullptr),
          size(1) {}
    int key;
    int priority;
    Node* left_child = nullptr;
    Node* right_child = nullptr;
    int size = 0;
  };

  static std::pair<Node*, Node*> Split(Node* node, int elem);
  static Node* Merge(Node* left, Node* right);
  static int GetSize(Node* node);
  static void UpdateSize(Node* node);

  void ClearTreap(Node* node);

  Node* root_ = nullptr;
};

void Treap::ClearTreap(Treap::Node* node) {
  if (node == nullptr) {
    return;
  }
  ClearTreap(node->left_child);
  ClearTreap(node->right_child);
  delete node;
}

std::pair<Treap::Node*, Treap::Node*> Treap::Split(Node* node, int elem) {
  if (node == nullptr) {
    return {nullptr, nullptr};
  }
  if (node->key <= elem) {
    std::pair<Node*, Node*> split_return = Split(node->right_child, elem);
    node->right_child = split_return.first;
    UpdateSize(node);
    UpdateSize(split_return.second);
    return {node, split_return.second};
  }
  std::pair<Node*, Node*> split_return = Split(node->left_child, elem);
  node->left_child = split_return.second;
  UpdateSize(node);
  UpdateSize(split_return.first);
  return {split_return.first, node};
}

Treap::Node* Treap::Merge(Treap::Node* left, Treap::Node* right) {
  if (left == nullptr) {
    return right;
  }
  if (right == nullptr) {
    return left;
  }
  if (left->priority < right->priority) {
    left->right_child = Merge(left->right_child, right);
    UpdateSize(left);
    return left;
  }
  right->left_child = Merge(left, right->left_child);
  UpdateSize(right);
  return right;
}

int Treap::GetSize(Treap::Node* node) {
  if (node == nullptr) {
    return 0;
  }
  return node->size;
}

void Treap::UpdateSize(Node* node) {
  if (node != nullptr) {
    node->size = GetSize(node->left_child) + GetSize(node->right_child) + 1;
  }
}

void Treap::Insert(int elem) {
  Node* new_node = new Node(elem);
  std::pair<Node*, Node*> split_return = Split(root_, elem);
  root_ = Merge(split_return.first, Merge(new_node, split_return.second));
}

void Treap::Delete(int elem) {
  std::pair<Node*, Node*> split_return_left = Split(root_, elem - 1);
  std::pair<Node*, Node*> split_return_right =
      Split(split_return_left.second, elem);
  ClearTreap(split_return_right.first);
  root_ = Merge(split_return_left.first, split_return_right.second);
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

int main() {
  int amount = 0;
  std::cin >> amount;
  int in_last_row = pow(2, MyLog(amount));
  std::vector<std::vector<Treap>> tree
}