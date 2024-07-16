#include <stdlib.h>

#include <iostream>
#include <string>

namespace Constants {
const int kMaxPriority = 1000000000;
const int kNone = 1000000000 + 33;
}  // namespace Constants

class Treap {
 public:
  Treap() = default;

  ~Treap() { ClearTreap(root_); }

  void Insert(int elem);
  void Delete(int elem);
  bool Exists(int elem);
  int Next(int elem);
  int Prev(int elem);
  int Kth(int order);

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

  bool Exists(Node* node, int elem);
  int Next(Node* node, int elem, int prob_ans = Constants::kNone);
  int Prev(Node* node, int elem, int prob_ans = Constants::kNone);
  int Kth(Node* node, int order);

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

bool Treap::Exists(int elem) { return Treap::Exists(root_, elem); }

bool Treap::Exists(Treap::Node* node, int elem) {
  if (node == nullptr) {
    return false;
  }
  if (node->key == elem) {
    return true;
  }
  if (elem < node->key) {
    return Exists(node->left_child, elem);
  }
  return Exists(node->right_child, elem);
}

void Treap::Insert(int elem) {
  if (Exists(elem)) {
    return;
  }
  Node* new_node = new Node(elem);
  std::pair<Node*, Node*> split_return = Split(root_, elem);
  root_ = Merge(split_return.first, Merge(new_node, split_return.second));
}

void Treap::Delete(int elem) {
  if (Exists(elem)) {
    std::pair<Node*, Node*> split_return_left = Split(root_, elem - 1);
    std::pair<Node*, Node*> split_return_right =
        Split(split_return_left.second, elem);
    ClearTreap(split_return_right.first);
    root_ = Merge(split_return_left.first, split_return_right.second);
  }
}

int Treap::Next(int elem) { return Treap::Next(root_, elem); }

int Treap::Next(Treap::Node* node, int elem, int prob_ans) {
  if (node == nullptr) {
    return prob_ans;
  }
  if (elem < node->key) {
    prob_ans = node->key;
    return Next(node->left_child, elem, prob_ans);
  }
  return Next(node->right_child, elem, prob_ans);
}

int Treap::Prev(int elem) { return Treap::Prev(root_, elem); }

int Treap::Prev(Treap::Node* node, int elem, int prob_ans) {
  if (node == nullptr) {
    return prob_ans;
  }
  if (elem > node->key) {
    prob_ans = node->key;
    return Prev(node->right_child, elem, prob_ans);
  }
  return Prev(node->left_child, elem, prob_ans);
}

int Treap::Kth(int order) {
  if (order > root_->size) {
    return Constants::kNone;
  }
  return Treap::Kth(root_, order);
}

int Treap::Kth(Treap::Node* node, int order) {
  if (node == nullptr) {
    return Constants::kNone;
  }
  if (GetSize(node->left_child) + 1 == order) {
    return node->key;
  }
  if (order <= GetSize(node->left_child)) {
    return Kth(node->left_child, order);
  }
  return Kth(node->right_child, order - GetSize(node->left_child) - 1);
}

void OutPut(Treap& treap, std::string& command, int elem) {
  if (command == "insert") {
    treap.Insert(elem);
  }
  if (command == "delete") {
    treap.Delete(elem);
  }
  if (command == "exists") {
    bool ans = treap.Exists(elem);
    if (ans) {
      std::cout << "true"
                << "\n";
    } else {
      std::cout << "false"
                << "\n";
    }
  }
  if (command == "next") {
    int ans = treap.Next(elem);
    if (ans == Constants::kNone) {
      std::cout << "none"
                << "\n";
    } else {
      std::cout << ans << "\n";
    }
  }
  if (command == "prev") {
    int ans = treap.Prev(elem);
    if (ans == Constants::kNone) {
      std::cout << "none"
                << "\n";
    } else {
      std::cout << ans << "\n";
    }
  }
  if (command == "kth") {
    int ans = treap.Kth(elem + 1);
    if (ans == Constants::kNone) {
      std::cout << "none"
                << "\n";
    } else {
      std::cout << ans << "\n";
    }
  }
}

int main() {
  Treap treap;
  std::string command;
  while (std::cin >> command) {
    int elem;
    std::cin >> elem;
    OutPut(treap, command, elem);
  }
}