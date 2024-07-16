#include <stdlib.h>

#include <iostream>
#include <set>

namespace Constants {
const int kMaxPriority = 1000000000;
}

class Treap {
 public:
  Treap() = default;

  ~Treap() { ClearTreap(root_); }

  void Insert(int elem);
  int GreaterThan(int elem);
  int LessThan(int elem);

 private:
  struct Node {
    explicit Node(int key)
        : key(key),
          priority(rand() % Constants::kMaxPriority),
          size(1),
          left_child(nullptr),
          right_child(nullptr) {}
    int key;
    int priority;
    int size = 0;
    Node* left_child = nullptr;
    Node* right_child = nullptr;
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

int Treap::GreaterThan(int elem) {
  std::pair<Node*, Node*> split_return = Split(root_, elem);
  int answer = GetSize(split_return.second);
  root_ = Merge(split_return.first, split_return.second);
  return answer;
}

int Treap::LessThan(int elem) {
  std::pair<Node*, Node*> split_return = Split(root_, elem - 1);
  int answer = GetSize(split_return.first);
  root_ = Merge(split_return.first, split_return.second);
  return answer;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int lenght = 0;
  int width = 0;
  std::cin >> lenght >> width;
  int flowers_amount = 0;
  std::cin >> flowers_amount;
  int questions = 0;
  std::cin >> questions;
  std::set<std::pair<int, int>> flowers;
  for (int i = 0; i < flowers_amount; ++i) {
    int x_coord = 0;
    std::cin >> x_coord;
    int y_coord = 0;
    std::cin >> y_coord;
    flowers.insert(std::make_pair(x_coord, y_coord));
  }
  Treap x_1;
  Treap x_2;
  Treap y_1;
  Treap y_2;
  int watering = 0;
  for (int i = 0; i < questions; ++i) {
    int command = 0;
    std::cin >> command;
    if (command == 1) {
      ++watering;
      int x_1_coord = 0;
      int y_1_coord = 0;
      int x_2_coord = 0;
      int y_2_coord = 0;
      std::cin >> x_1_coord >> y_1_coord >> x_2_coord >> y_2_coord;
      x_1.Insert(x_1_coord + 1);
      x_2.Insert(x_2_coord);
      y_1.Insert(y_1_coord + 1);
      y_2.Insert(y_2_coord);
    } else {
      int x_coord = 0;
      std::cin >> x_coord;
      int y_coord = 0;
      std::cin >> y_coord;
      int answer = 1;
      if (flowers.contains(std::make_pair(x_coord, y_coord))) {
        answer = 0;
      }
      answer += x_1.GreaterThan(x_coord);
      answer += x_2.LessThan(x_coord);
      answer += watering - y_2.LessThan(y_coord) - y_1.GreaterThan(y_coord);
      answer %= 2;
      if (answer == 0) {
        std::cout << "YES"
                  << "\n";
      } else {
        std::cout << "NO"
                  << "\n";
      }
    }
  }
}