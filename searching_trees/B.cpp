#include <iostream>

namespace Constants {
const int kMod = 1000000000;
}

struct Node {
  explicit Node(int elem)
      : element(elem), height(1), left_child(nullptr), right_child(nullptr) {}
  int element;
  int height;
  Node* left_child = nullptr;
  Node* right_child = nullptr;
};

class AVLtree {
 public:
  AVLtree() = default;

  ~AVLtree() { ClearTree(parent_); }

  void Insert(int elem);
  int Next(int elem);

 private:
  static int GetHeight(Node* node);
  static int GetDelta(Node* node);
  static void UpdateHeight(Node*& node);
  static void LeftSmallRotation(Node*& node);
  static void RightSmallRotation(Node*& node);
  static void LeftBigRotation(Node*& node);
  static void RightBigRotation(Node*& node);
  static void Balance(Node*& node);
  void Insert(Node*& node, int elem);
  int Next(Node* node, int elem, int probable_ans = -1);
  void ClearTree(Node*& node);

  Node* parent_ = nullptr;
};

void AVLtree::Insert(int elem) { Insert(parent_, elem); }

int AVLtree::Next(int elem) { return Next(parent_, elem); }

int AVLtree::GetHeight(Node* node) {
  if (node == nullptr) {
    return 0;
  }
  return node->height;
}

void AVLtree::UpdateHeight(Node*& node) {
  node->height =
      1 + std::max(GetHeight(node->left_child), GetHeight(node->right_child));
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

  node = new_parrent_node;
}

void AVLtree::RightSmallRotation(Node*& node) {
  Node* new_parrent_node = node->left_child;
  Node* node_left_chid = new_parrent_node->right_child;
  new_parrent_node->right_child = node;
  node->left_child = node_left_chid;

  UpdateHeight(new_parrent_node);
  UpdateHeight(node);

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
  Balance(node);
}

int AVLtree::Next(Node* node, int elem, int probable_ans) {
  if (node == nullptr) {
    return probable_ans;
  }
  if (elem <= node->element) {
    probable_ans = node->element;
    return Next(node->left_child, elem, probable_ans);
  }
  return Next(node->right_child, elem, probable_ans);
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
  int last_next_result = 0;
  for (int i = 0; i < questions; ++i) {
    char command;
    int elem;
    std::cin >> command;
    std::cin >> elem;
    if (command == '+') {
      if (prev_command == '+') {
        avl.Insert(elem);
      } else {
        avl.Insert((elem + last_next_result) % Constants::kMod);
      }
    } else {
      last_next_result = avl.Next(elem);
      std::cout << last_next_result << "\n";
    }
    prev_command = command;
  }
}