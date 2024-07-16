#include <iostream>
#include <vector>
#include <cmath>

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

int MyLogDown(int number) {
  int answer = 0;
  while (number > 1) {
    number /= 2;
    ++answer;
  }
  return answer;
}

void GetEulerBypass(int cur_vert, int cur_depth, std::vector<std::vector<int>>& adjacency_matrix, std::vector<int>& depth, std::vector<int>& euler_bypass, std::vector<bool>& is_visited) {
  depth[cur_vert] = cur_depth;
  euler_bypass.push_back(cur_vert);
  is_visited[cur_vert] = true;
  for (int neighbour: adjacency_matrix[cur_vert]) {
    if (!is_visited[neighbour]) {
      GetEulerBypass(neighbour, cur_depth + 1,adjacency_matrix, depth, euler_bypass, is_visited);
      euler_bypass.push_back(cur_vert);
    }
  }
}

std::vector<std::vector<std::pair<int, int>>> BuildSparseTable(std::vector<int>& euler_bypass, std::vector<int>& depth) {
  int len = euler_bypass.size();
  if (len == 1) {
    ++len;
  }
  std::vector<std::vector<std::pair<int, int>>> sparse_table(MyLogUp(len), std::vector<std::pair<int, int>>(len));
  for (int i = 0; i < len; ++i) {
    sparse_table[0][i] = {depth[euler_bypass[i]], euler_bypass[i]};
  }
  for (int i = 1; i < MyLogUp(len); ++i) {
    for (int j = 0; j + pow(2, i) <= len; ++j) {
      sparse_table[i][j] = std::min(sparse_table[i - 1][j], sparse_table[i - 1][j + pow(2, i - 1)]);
    }
  }
  return sparse_table;
}

int GetLCADepth(int first, int second, std::vector<std::vector<std::pair<int, int>>>& sparse_table, std::vector<int>& first_entry) {
  int left = first_entry[first];
  int right = first_entry[second];
  if (left > right) {
    std::swap(left, right);
  }
  int max_deg = MyLogDown(right - left + 1);
  int answer = std::min(sparse_table[max_deg][left].first, sparse_table[max_deg][right - pow(2, max_deg) + 1].first);
  return answer;
}

int main() {
  int vertex_amount;
  std::cin >> vertex_amount;
  std::vector<std::vector<int>> adjacency_matrix(vertex_amount);
  for (int i = 0; i < vertex_amount - 1; ++i) {
    int start, finish;
    std::cin >> start >> finish;
    --start;
    --finish;
    adjacency_matrix[start].push_back(finish);
    adjacency_matrix[finish].push_back(start);
  }
  std::vector<int> depth(vertex_amount);
  std::vector<int> euler_bypass;
  std::vector<bool> is_visited(vertex_amount, false);
  GetEulerBypass(0, 1, adjacency_matrix, depth, euler_bypass, is_visited);
  std::vector<int> first_entry(vertex_amount, -1);
  for (size_t i = 0; i < euler_bypass.size(); ++i) {
    if (first_entry[euler_bypass[i]] == -1) {
      first_entry[euler_bypass[i]] = i;
    }
  }
  std::vector<std::vector<std::pair<int, int>>> sparse_table = BuildSparseTable(euler_bypass, depth);
  int questions_amount;
  std::cin >> questions_amount;
  for (int i = 0; i < questions_amount; ++i) {
    int start, finish;
    std::cin >> start >> finish;
    --start;
    --finish;
    int lca_depth = GetLCADepth(start, finish, sparse_table, first_entry);
    std::cout << depth[start] - lca_depth + depth[finish] - lca_depth << "\n";
  }
}