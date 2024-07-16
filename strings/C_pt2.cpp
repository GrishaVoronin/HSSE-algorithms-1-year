#include <iostream>
#include <vector>

const int alphabet_size = 26;

struct Word {
  Word(std::string& str, bool is_start, int number) :
  str(str), is_start(is_start), number(number) {}
  std::string str;
  bool is_start;
  int number;
};

struct Node {
  Node() = default;
  bool is_terminal = false;
  std::vector<Node*> children = std::vector<Node*>(26, nullptr);
  std::vector<Word> terminated_words;
};

class Trie {
 public:
  void AddString(const Word& cur_word) {
    Node* current_node = root;
    for (char symbol: cur_word.str) {
      symbol -= 'a';
      if (current_node->children[symbol] == nullptr) {
        current_node->children[symbol] = new Node();
      }
      current_node = current_node->children[symbol];
    }
    current_node->is_terminal = true;
    current_node->terminated_words.push_back(cur_word);
  }

  const std::vector<Word>& ContainsString(const std::string& str) {
    Node* current_node = root;
    for (char symbol: str) {
      symbol -= 'a';
      if (current_node->children[symbol] == nullptr) {
        return current_node->terminated_words;
      }
      current_node = current_node->children[symbol];
    }
    return current_node->terminated_words;
  }

 private:
  Node* root = new Node();
};

bool is_palindrom(const std::string& str) {
  for (size_t i = 0; i < str.size() / 2; ++i) {
    if (str[i] != str[str.size() - 1 - i]) {
      return false;
    }
  }
  return true;
}

void word_handle(std::string& str, Trie& trie, int word_number) {
  std::reverse(str.begin(), str.end());
  trie.AddString({str, true, word_number});
  std::cout << str << "\n";
  for (size_t i = 1; i < str.size(); ++i) {
    if (is_palindrom(str.substr(0, i))) {
      std::string s = str.substr(i, str.size());
      std::cout << s << "\n";
      trie.AddString({s, true, word_number});
    }
  }
  for (size_t i = 1; i < str.size(); ++i) {
    if (is_palindrom(str.substr(i, str.size()))) {
      std::string s = str.substr(0, i);
      std::cout << s << "\n";
      trie.AddString({s, false, word_number});
    }
  }
}

int main() {
  Trie trie;
  int words_amount;
  std::cin >> words_amount;
  std::vector<std::string> words;
  for (int i = 0; i < words_amount; ++i) {
    std::string word;
    std::cin >> word;
    words.push_back(word);
    word_handle(word, trie, i + 1);
  }
  std::vector<std::pair<int, int>> pairs;
  for (int i = 0; i < words_amount; ++i) {
    for (auto word: trie.ContainsString(words[i])) {
      if (word.number != i + 1) {
        if (word.is_start) {
          pairs.emplace_back(word.number, i + 1);
        } else {
          pairs.emplace_back(i + 1, word.number);
        }
      }
    }
  }
  std::cout << pairs.size() << "\n";
  for (size_t i = 0; i < pairs.size(); ++i) {
    std::cout << pairs[i].first << ' ' << pairs[i].second << "\n";
  }
}