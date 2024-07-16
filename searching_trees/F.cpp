#include <iostream>

namespace Constants {
const int kMod = 1000000000;
}

struct Node {
  explicit Node(int elem)
      : element(elem),
        height(1),
        sum(elem),
        left_child(nullptr),
        right_child(nullptr) {}
  int element;
  int height;
  long long sum = 0;
  Node* left_child = nullptr;
  Node* right_child = nullptr;
};

class AVLtree {
 public:
  AVLtree() = default;

  ~AVLtree() { ClearTree(parent_); }

  void Insert(int elem);
  long long Sum(int left, int right);

 private:
  static int GetHeight(Node* node);
  static long long GetSum(Node* node);
  static int GetDelta(Node* node);
  static void UpdateHeight(Node*& node);
  static void UpdateSum(Node*& node);
  static void LeftSmallRotation(Node*& node);
  static void RightSmallRotation(Node*& node);
  static void LeftBigRotation(Node*& node);
  static void RightBigRotation(Node*& node);
  static void Balance(Node*& node);
  void Insert(Node*& node, int elem);
  long long GreaterThan(Node* node, int elem, long long probable_ans = 0);
  long long LessThan(Node* node, int elem, long long probable_ans = 0);
  void ClearTree(Node*& node);

  Node* parent_ = nullptr;
};

void AVLtree::Insert(int elem) { Insert(parent_, elem); }

int AVLtree::GetHeight(Node* node) {
  if (node == nullptr) {
    return 0;
  }
  return node->height;
}

long long AVLtree::GetSum(Node* node) {
  if (node == nullptr) {
    return 0;
  }
  return node->sum;
}

void AVLtree::UpdateHeight(Node*& node) {
  node->height =
      1 + std::max(GetHeight(node->left_child), GetHeight(node->right_child));
}

void AVLtree::UpdateSum(Node*& node) {
  node->sum =
      node->element + GetSum(node->left_child) + GetSum(node->right_child);
}

int AVLtree::GetDelta(Node* node) {
  return GetHeight(node->left_child) - GetHeight(node->right_child);
}

void AVLtree::LeftSmallRotation(Node*& node) {
  Node* new_parrent_node = node->right_child;
  Node* node_right_child = new_parrent_node->left_child;
  new_parrent_node->left_child = node;
  node->right_child = node_right_child;

  UpdateHeight(new_parrent_node);
  UpdateHeight(node);
  UpdateSum(node);
  UpdateSum(new_parrent_node);

  node = new_parrent_node;
}

void AVLtree::RightSmallRotation(Node*& node) {
  Node* new_parrent_node = node->left_child;
  Node* node_left_chid = new_parrent_node->right_child;
  new_parrent_node->right_child = node;
  node->left_child = node_left_chid;

  UpdateHeight(new_parrent_node);
  UpdateHeight(node);
  UpdateSum(node);
  UpdateSum(new_parrent_node);

  node = new_parrent_node;
}

void AVLtree::LeftBigRotation(Node*& node) {
  RightSmallRotation(node->right_child);
  LeftSmallRotation(node);
}

void AVLtree::RightBigRotation(Node*& node) {
  LeftSmallRotation(node->left_child);
  RightSmallRotation(node);
}

void AVLtree::Balance(Node*& node) {
  int delta = GetDelta(node);
  if (delta == -2) {
    if (GetDelta(node->right_child) < 0) {
      LeftSmallRotation(node);
    } else {
      LeftBigRotation(node);
    }
  } else if (delta == 2) {
    if (GetDelta(node->left_child) > 0) {
      RightSmallRotation(node);
    } else {
      RightBigRotation(node);
    }
  }
}

void AVLtree::Insert(Node*& node, int elem) {
  if (node == nullptr) {
    Node* new_node = new Node(elem);
    node = new_node;
  } else if (elem < node->element) {
    Insert(node->left_child, elem);
  } else if (elem > node->element) {
    Insert(node->right_child, elem);
  }

  UpdateHeight(node);
  UpdateSum(node);
  Balance(node);
}

long long AVLtree::GreaterThan(Node* node, int elem, long long probable_ans) {
  if (node == nullptr) {
    return probable_ans;
  }
  if (elem < node->element) {
    probable_ans += GetSum(node) - GetSum(node->left_child);
    return GreaterThan(node->left_child, elem, probable_ans);
  }
  return GreaterThan(node->right_child, elem, probable_ans);
}

long long AVLtree::LessThan(Node* node, int elem, long long probable_ans) {
  if (node == nullptr) {
    return probable_ans;
  }
  if (elem > node->element) {
    probable_ans += GetSum(node) - GetSum(node->right_child);
    return LessThan(node->right_child, elem, probable_ans);
  }
  return LessThan(node->left_child, elem, probable_ans);
}

long long AVLtree::Sum(int left, int right) {
  if (parent_ != nullptr) {
    return parent_->sum - GreaterThan(parent_, right) - LessThan(parent_, left);
  }
  return 0;
}

void AVLtree::ClearTree(Node*& node) {
  if (node == nullptr) {
    return;
  }
  ClearTree(node->left_child);
  ClearTree(node->right_child);
  delete node;
}

int main() {
  std::cin.tie(nullptr);
  int questions = 0;
  std::cin >> questions;
  AVLtree avl;
  char prev_command = '+';
  long long last_sum_result = 0;
  int left;
  int right;
  char command;
  for (int i = 0; i < questions; ++i) {
    std::cin >> command;
    if (command == '+') {
      int elem;
      std::cin >> elem;
      if (prev_command == '+') {
        avl.Insert(elem);
      } else {
        avl.Insert((elem + last_sum_result) % Constants::kMod);
      }
    } else {
      std::cin >> left;
      std::cin >> right;
      last_sum_result = avl.Sum(left, right);
      std::cout << last_sum_result << "\n";
    }
    prev_command = command;
  }
}