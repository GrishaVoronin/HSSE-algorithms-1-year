#include <iostream>

class Treap {
 public:
  Treap() = default;

  ~Treap() { ClearTreap(root_); }

  void Insert(int key, int priority) { Insert(last_added_, key, priority); }

  void Print() { Print(root_); }

 private:
  struct Node {
    explicit Node(int key, int priority)
        : key(key),
          priority(priority),
          parent(nullptr),
          left_child(nullptr),
          right_child(nullptr) {}
    int key;
    int priority;
    Node* parent = nullptr;
    Node* left_child = nullptr;
    Node* right_child = nullptr;
  };

  void ClearTreap(Node*& node);
  void Insert(Node*& prob_parent, int key, int priority);
  void Print(Node* node);

  Node* root_ = nullptr;
  Node* last_added_ = nullptr;
};

void Treap::Insert(Node*& prob_parent, int key, int priority) {
  if (prob_parent == nullptr) {
    Node* new_node = new Node(key, priority);
    if (root_ != nullptr) {
      root_->parent = new_node;
    }
    new_node->left_child = root_;
    root_ = new_node;
    last_added_ = new_node;
  } else {
    if (priority < prob_parent->priority) {
      Insert(prob_parent->parent, key, priority);
    } else {
      Node* new_node = new Node(key, priority);
      new_node->left_child = prob_parent->right_child;
      new_node->parent = prob_parent;
      prob_parent->right_child = new_node;
      if (new_node->left_child != nullptr) {
        new_node->left_child->parent = new_node;
      }
      last_added_ = new_node;
    }
  }
}

void Treap::ClearTreap(Treap::Node*& node) {
  if (node == nullptr) {
    return;
  }
  ClearTreap(node->left_child);
  ClearTreap(node->right_child);
  delete node;
}

void Treap::Print(Treap::Node* node) {
  if (node == nullptr) {
    return;
  }
  Print(node->left_child);
  if (node->parent == nullptr) {
    std::cout << 0 << ' ';
  } else {
    std::cout << node->parent->key << ' ';
  }
  if (node->left_child == nullptr) {
    std::cout << 0 << ' ';
  } else {
    std::cout << node->left_child->key << ' ';
  }
  if (node->right_child == nullptr) {
    std::cout << 0 << ' ';
  } else {
    std::cout << node->right_child->key << ' ';
  }
  std::cout << "\n";
  Print(node->right_child);
}

int main() {
  int numbers;
  std::cin >> numbers;
  Treap treap;
  for (int i = 1; i <= numbers; ++i) {
    int key;
    std::cin >> key;
    int priority;
    std::cin >> priority;
    treap.Insert(i, priority);
  }
  std::cout << "YES"
            << "\n";
  treap.Print();
}