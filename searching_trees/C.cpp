#include <iostream>
#include <string>

struct Node {
  explicit Node(std::string key, std::string elem)
      : key(key),
        element(elem),
        height(1),
        left_child(nullptr),
        right_child(nullptr) {}
  std::string key;
  std::string element;
  int height;
  Node* left_child = nullptr;
  Node* right_child = nullptr;
};

class AVLtree {
 public:
  AVLtree() = default;

  ~AVLtree() { ClearTree(parent_); }

  void Insert(std::string& key, std::string& elem);
  void GetElement(std::string& key);

 private:
  static int GetHeight(Node* node);
  static int GetDelta(Node* node);
  static void UpdateHeight(Node*& node);
  static void LeftSmallRotation(Node*& node);
  static void RightSmallRotation(Node*& node);
  static void LeftBigRotation(Node*& node);
  static void RightBigRotation(Node*& node);
  static void Balance(Node*& node);
  void Insert(Node*& node, const std::string& key, const std::string& elem);
  void GetElemet(Node* node, const std::string& key_1);
  void ClearTree(Node* node);

  Node* parent_ = nullptr;
};

void AVLtree::Insert(std::string& key, std::string& elem) {
  Insert(parent_, key, elem);
}

void AVLtree::GetElement(std::string& key) { GetElemet(parent_, key); }

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

void AVLtree::Insert(Node*& node, const std::string& key,
                     const std::string& elem) {
  if (node == nullptr) {
    Node* new_node = new Node(key, elem);
    node = new_node;
  } else if (key < node->key) {
    Insert(node->left_child, key, elem);
  } else if (key > node->key) {
    Insert(node->right_child, key, elem);
  }

  UpdateHeight(node);
  Balance(node);
}

void AVLtree::GetElemet(Node* node, const std::string& key_1) {
  if (node == nullptr) {
    return;
  }
  if (node->key == key_1) {
    std::cout << node->element << "\n";
  } else if (key_1 < node->key) {
    GetElemet(node->left_child, key_1);
  } else {
    GetElemet(node->right_child, key_1);
  }
}

void AVLtree::ClearTree(Node* node) {
  if (node == nullptr) {
    return;
  }
  ClearTree(node->left_child);
  ClearTree(node->right_child);
  delete node;
}

int main() {
  std::cin.tie(nullptr);
  int amout_of_pair;
  std::cin >> amout_of_pair;
  AVLtree avl;
  for (int i = 0; i < amout_of_pair; ++i) {
    std::string driver;
    std::cin >> driver;
    std::string car;
    std::cin >> car;
    avl.Insert(driver, car);
    avl.Insert(car, driver);
  }
  int questions;
  std::cin >> questions;
  for (int i = 0; i < questions; ++i) {
    std::string key;
    std::cin >> key;
    avl.GetElement(key);
  }
}