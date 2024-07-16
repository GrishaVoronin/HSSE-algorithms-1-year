#include <iostream>
#include <vector>
#include <unordered_map>

int MyLogUp(int number) {
  int add = 0;
  int answer = 0;
  while (number > 1) {
    if (number % 2 == 1) {
      add = 1;
    }
    number /= 2;
    ++answer;
  }
  return answer + add;
}

bool IsAncestor(int u, int v, std::vector<std::vector<int>>& dp, std::unordered_map<int, int>& depth, int question_number) {
  if (depth[u] > depth[v]) {
    return false;
  }
  int delta = depth[v] - depth[u];
  for (int i = MyLogUp(question_number) - 1; i >= 0; --i) {
    if (delta >= (1<<i)) {
      v = dp[i][v];
      delta -= 1<<i;
    }
  }
  if (u == v) {
    return true;
  }
  return false;
}

int GetLCA(int u, int v, std::vector<std::vector<int>>& dp, std::unordered_map<int, int>& depth, int question_number) {
  if (IsAncestor(u, v, dp, depth, question_number)) {
    return u;
  }
  for (int i = MyLogUp(question_number) - 1; i >= 0; --i) {
    if (!IsAncestor(dp[i][u], v, dp, depth, question_number)) {
      u = dp[i][u];
    }
  }
  return dp[0][u];
}

int GetAnswer(int lca, std::unordered_map<int, int>& parent, std::unordered_map<int, bool>& is_deleted) {
  if (!is_deleted.contains(lca)) {
    return lca;
  }
  parent[lca] = GetAnswer(parent[lca], parent, is_deleted);
  return parent[lca];
}

int main() {
  int questions_amount;
  std::cin >> questions_amount;
  int cur_company_number = 1;
  std::unordered_map<int, int> parent;
  std::unordered_map<int, bool> is_deleted;
  std::unordered_map<int, int> depth;
  depth[1] = 1;
  std::vector<std::vector<int>> dp(MyLogUp(questions_amount), std::vector<int>(questions_amount + 1));
  for (int i = 0; i < MyLogUp(questions_amount); ++i) {
    dp[i][1] = 1;
  }
  for (int i = 0; i < questions_amount; ++i) {
    char command;
    std::cin >> command;
    if (command == '+') {
      ++cur_company_number;
      int cur_parent;
      std::cin >> cur_parent;
      parent[cur_company_number] = cur_parent;
      dp[0][cur_company_number] = cur_parent;
      depth[cur_company_number] = depth[cur_parent] + 1;
      for (int j = 1; j < MyLogUp(questions_amount); ++j) {
        dp[j][cur_company_number] = dp[j - 1][dp[j - 1][cur_company_number]];
      }
    } else if (command == '-') {
      int company_to_delete;
      std::cin >> company_to_delete;
      is_deleted[company_to_delete] = true;
    } else {
      int first, second;
      std::cin >> first >> second;
      int lca = GetLCA(first, second, dp, depth, questions_amount);
      std::cout << GetAnswer(lca, parent, is_deleted) << "\n";
    }
  }
}