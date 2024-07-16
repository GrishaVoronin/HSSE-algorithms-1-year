#include <iostream>
#include <vector>
#include <string>

int GetMaxPrefix(std::string& string) {
  std::vector<int> pi_func(string.size(), 0);
  for (int i = 1; i < string.size(); ++i) {
    int current_prefix = pi_func[i - 1];
    while (current_prefix > 0 and string[i] != string[current_prefix]) {
      current_prefix = pi_func[current_prefix - 1];
    }
    pi_func[i] = current_prefix;
    if (string[i] == string[current_prefix]) {
      ++pi_func[i];
    }
  }
  return pi_func[pi_func.size() - 1];
}

int main() {
  int words_amount;
  std::cin >> words_amount;
  std::string str;
  std::cin >> str;
  for (int i = 1; i < words_amount; ++i) {
    std::string next;
    std::cin >> next;
    std::string concat = next + "#" + str.substr(std::max(0, static_cast<int>(str.size()) - static_cast<int>(next.size())), str.size());
    int max_prefix = GetMaxPrefix(concat);
    str += next.substr(max_prefix, next.size());
  }
  std::cout << str;
}