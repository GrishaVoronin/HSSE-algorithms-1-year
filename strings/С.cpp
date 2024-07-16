#include <iostream>
#include <vector>

bool is_palindrom(const std::string& str, int start, int finish) {
  while (start < finish) {
    if (str[start] != str[finish]) {
      return false;
    }
    ++start;
    --finish;
  }
  return true;
}

const int alphabet_size = 26;

struct Node {
  Node() = default;
  int terminated_word_number = -1;
  std::vector<Node*> children = std::vector<Node*>(alphabet_size, nullptr);
  std::vector<int> terminated_words;
};

class Trie {
 public:
  void AddString(std::string& str, int word_number) {
    Node* current_node = root;
    for (int i = str.size() - 1; i >= 0; --i) {
      int symbol = str[i] - 'a';
      if (current_node->children[symbol] == nullptr) {
        current_node->children[symbol] = new Node();
      }
      if (is_palindrom(str, 0, i)) {
        current_node->terminated_words.push_back(word_number);
      }
      current_node = current_node->children[symbol];
    }
    current_node->terminated_word_number = word_number;
    current_node->terminated_words.push_back(word_number);
  }

  std::vector<int> ConjugateString(const std::string& str, int word_number) {
    Node* current_node = root;
    std::vector<int> answer;
    for (int i = 0; i < str.size(); ++i) {
      if (is_palindrom(str, i, str.size() - 1) && current_node->terminated_word_number != word_number && current_node->terminated_word_number != -1) {
        answer.push_back(current_node->terminated_word_number);
      }
      current_node = current_node->children[str[i] - 'a'];
      if (current_node == nullptr) {
        return answer;
      }
    }

    for (int i : current_node->terminated_words) {
      if (i != word_number) {
        answer.push_back(i);
      }
    }
    return answer;
  }

 private:
  Node* root = new Node();
};

int main() {
  Trie trie;
  int words_amount;
  std::cin >> words_amount;
  std::vector<std::string> words;
  for (int i = 0; i < words_amount; ++i) {
    std::string word;
    std::cin >> word;
    words.push_back(word);
    trie.AddString(word, i);
  }
  std::vector<std::pair<int, int>> pairs;
  for (int i = 0; i < words_amount; ++i) {
    std::vector<int> conjugate_words = trie.ConjugateString(words[i], i);
    for (auto w: conjugate_words) {
      pairs.emplace_back(i + 1, w + 1);
    }
  }
  std::cout << pairs.size() << "\n";
  for (size_t i = 0; i < pairs.size(); ++i) {
    std::cout << pairs[i].first << ' ' << pairs[i].second << "\n";
  }
}