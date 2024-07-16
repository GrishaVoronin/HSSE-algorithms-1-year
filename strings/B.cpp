#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> ZetFunction(std::string s) {
  std::vector<int> z(s.size(), 0);
  int left = 0;
  int right = 0;
  for (int i = 1; i < s.size(); ++i) {
    z[i] = std::max(0, std::min(right - i, z[i - left]));
    while (s[z[i]] == s[i + z[i]] and i + z[i] < s.size()) {
      ++z[i];
    }
    if (i + z[i] > right) {
      left = i, right = i + z[i];
    }
  }
  return z;
}

int main() {
  std::string pattern;
  std::cin >> pattern;
  std::string text;
  std::cin >> text;
  std::string string = pattern + "#" + text;
  std::reverse(pattern.begin(), pattern.end());
  std::reverse(text.begin(), text.end());
  std::string reversed_string = pattern + "#" + text;
  std::vector<int> z_func = ZetFunction(string);
  std::vector<int> z_func_reversed = ZetFunction(reversed_string);
  std::vector<int> vec_1;
  std::vector<int> vec_2;
  for (size_t i = pattern.size() + 1; i < pattern.size() + text.size() + 1; ++i) {
    vec_1.push_back(z_func[i]);
    vec_2.push_back(z_func_reversed[i]);
  }
  vec_2.push_back(0);
  std::reverse(vec_2.begin(), vec_2.end());
  vec_1.push_back(0);
  std::vector<int> vec(text.size() + 1, 0);
  for (int i = 0; i < vec_1.size(); ++i) {
    int sum = vec_1[i] + vec_2[i];
    if (sum >= pattern.size()) {
      ++vec[i - vec_2[i]];
      --vec[i - (pattern.size() - vec_1[i]) + 1];
    }
  }
  int answer = vec[0] > 0 ? 1 : 0;
  for (size_t i = 1; i < vec.size(); ++i) {
    vec[i] += vec[i - 1];
    if (vec[i] > 0) {
      ++answer;
    }
  }
  std::cout << answer;
}