#include <iostream>

namespace Constants {
const int kMaxPriority = 1000000000;
}

class Treap {
 public:
  Treap() = default;

  ~Treap() { ClearTreap(root_); }

  void Insert(int index, int elem);
  int GetMin(int left, int right);
  void Reverse(int left, int right);

 private:
  struct Node {
    explicit Node(int elem)
        : priority(rand() % Constants::kMaxPriority),
          elem(elem),
          min(elem),
          size(1),
          reversed(false),
          left_child(nullptr),
          right_child(nullptr) {}
    int priority;
    int elem;
    int min;
    int size;
    bool reversed;
    Node* left_child = nullptr;
    Node* right_child = nullptr;
  };

  static std::pair<Node*, Node*> Split(Node* node, int index);
  static Node* Merge(Node* left, Node* right);
  static int GetSize(Node* node);
  static int GetMin(Node* node);
  static void Update(Node* node);
  static void Push(Node* node);

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

void Treap::Push(Treap::Node* node) {
  if (node == nullptr) {
    return;
  }
  if (!node->reversed) {
    return;
  }
  Node* tmp = node->left_child;
  node->left_child = node->right_child;
  node->right_child = tmp;
  node->reversed = false;
  if (node->left_child != nullptr) {
    node->left_child->reversed ^= 1;
  }
  if (node->right_child != nullptr) {
    node->right_child->reversed ^= 1;
  }
}

int Treap::GetSize(Treap::Node* node) {
  if (node == nullptr) {
    return 0;
  }
  return node->size;
}

int Treap::GetMin(Treap::Node* node) {
  if (node == nullptr) {
    return Constants::kMaxPriority;
  }
  return node->min;
}

void Treap::Update(Treap::Node* node) {
  node->size = 1 + GetSize(node->left_child) + GetSize(node->right_child);
  node->min =
      std::min(std::min(GetMin(node->left_child), GetMin(node->right_child)),
               node->elem);
}

std::pair<Treap::Node*, Treap::Node*> Treap::Split(Node* node, int index) {
  if (node == nullptr) {
    return {nullptr, nullptr};
  }
  Push(node);
  int left_size = GetSize(node->left_child);
  if (left_size >= index) {
    std::pair<Node*, Node*> split_return = Split(node->left_child, index);
    node->left_child = split_return.second;
    Update(node);
    return std::make_pair(split_return.first, node);
  }
  std::pair<Node*, Node*> split_return =
      Split(node->right_child, index - left_size - 1);
  node->right_child = split_return.first;
  Update(node);
  return std::make_pair(node, split_return.second);
}

Treap::Node* Treap::Merge(Treap::Node* left, Treap::Node* right) {
  if (left == nullptr) {
    return right;
  }
  if (right == nullptr) {
    return left;
  }
  Push(left);
  Push(right);
  if (left->priority < right->priority) {
    left->right_child = Merge(left->right_child, right);
    Update(left);
    return left;
  }
  right->left_child = Merge(left, right->left_child);
  Update(right);
  return right;
}

void Treap::Insert(int index, int elem) {
  std::pair<Node*, Node*> split_return = Treap::Split(root_, index);
  Node* new_node = new Node(elem);
  root_ = Merge(Merge(split_return.first, new_node), split_return.second);
}

int Treap::GetMin(int left, int right) {
  std::pair<Node*, Node*> split_return_right_coord = Treap::Split(root_, right);
  std::pair<Node*, Node*> split_return_left_coord =
      Treap::Split(split_return_right_coord.first, left - 1);
  int min = GetMin(split_return_left_coord.second);
  root_ = Merge(
      Merge(split_return_left_coord.first, split_return_left_coord.second),
      split_return_right_coord.second);
  return min;
}

void Treap::Reverse(int left, int right) {
  std::pair<Node*, Node*> split_return_right_coord = Treap::Split(root_, right);
  std::pair<Node*, Node*> split_return_left_coord =
      Treap::Split(split_return_right_coord.first, left - 1);
  split_return_left_coord.second->reversed ^= 1;
  root_ = Merge(
      Merge(split_return_left_coord.first, split_return_left_coord.second),
      split_return_right_coord.second);
}

void InputHandler(int amount, int operations) {
  Treap tree;
  for (int i = 0; i < amount; ++i) {
    int elem = 0;
    std::cin >> elem;
    tree.Insert(i, elem);
  }
  for (int i = 0; i < operations; ++i) {
    int command = 0;
    std::cin >> command;
    if (command == 2) {
      int left = 0;
      int right = 0;
      std::cin >> left;
      std::cin >> right;
      std::cout << tree.GetMin(left, right) << "\n";
    } else {
      int left = 0;
      int right = 0;
      std::cin >> left;
      std::cin >> right;
      tree.Reverse(left, right);
    }
  }
}

int main() {
  std::cin.tie(nullptr);
  int amount = 0;
  std::cin >> amount;
  int operations;
  std::cin >> operations;
  InputHandler(amount, operations);
}