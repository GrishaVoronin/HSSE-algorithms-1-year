#include <iostream>
#include <vector>

const int alphabet_size = 26;

struct Node {
  Node() = default;
  bool is_terminal = false;
  std::vector<Node*> children = std::vector<Node*>(26, nullptr);
  Node* link = nullptr;
  std::vector<Node*> to = std::vector<Node*>(26, nullptr);
  std::vector<Node*> comp = std::vector<Node*>(26, nullptr);
};

class Trie {
 public:
  void AddString(const std::string& str) {
    Node* current_node = root;
    for (char symbol: str) {
      symbol -= 'a';
      if (current_node->children[symbol] == nullptr) {
        current_node->children[symbol] = new Node();
      }
      current_node = current_node->children[symbol];
    }
    current_node->is_terminal = true;
  }

 private:
  Node* root = new Node();
};


int main() {

}