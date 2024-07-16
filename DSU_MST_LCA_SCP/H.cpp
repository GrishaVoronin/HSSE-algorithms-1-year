#include <iostream>
#include <vector>
#include <unordered_map>
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

struct Edge{
  Edge (int start, int target, int edge_number) : start(start), target(target), edge_number(edge_number) {}
  int start;
  int target;
  int edge_number;
};

void FillTUp(std::vector<std::vector<Edge>>& adjacency_matrix, int cur_vert, int ancestor, int& timer, std::vector<int>& t_in, std::vector<int>& t_up, std::vector<bool>& is_visited) {
  is_visited[cur_vert] = true;
  ++timer;
  t_in[cur_vert] = timer;
  t_up[cur_vert] = timer;
  for (const Edge& edge : adjacency_matrix[cur_vert]) {
    if (edge.target == ancestor) {
      continue;
    }
    if (is_visited[edge.target]) {
      t_up[cur_vert] = std::min(t_up[cur_vert], t_in[edge.target]);
    } else {
      FillTUp(adjacency_matrix, edge.target, cur_vert, timer, t_in, t_up, is_visited);
      t_up[cur_vert] = std::min(t_up[cur_vert], t_up[edge.target]);
    }
  }
}

void BuildTree(std::vector<std::vector<Edge>>& adjacency_matrix, int cur_vert, std::vector<int>& bridges_amount_upper, std::unordered_map<int, bool>& is_bridge,
               std::vector<bool>& visited, std::vector<int>& parent, std::vector<int>& depth, std::vector<int>& euler_bypass, int cur_depth) {
  visited[cur_vert] = true;
  depth[cur_vert] = cur_depth;
  euler_bypass.push_back(cur_vert);
  for (const Edge& edge : adjacency_matrix[cur_vert]) {
    if (!visited[edge.target]) {
      parent[edge.target] = cur_vert;
      bridges_amount_upper[edge.target] = bridges_amount_upper[cur_vert];
      if (is_bridge.contains(edge.edge_number)) {
        ++bridges_amount_upper[edge.target];
      }
      BuildTree(adjacency_matrix, edge.target, bridges_amount_upper, is_bridge, visited, parent, depth, euler_bypass, cur_depth + 1);
      euler_bypass.push_back(cur_vert);
    }
  }
}

std::vector<std::vector<std::pair<int, int>>> BuildSparseTable(std::vector<int>& euler_bypass, std::vector<int>& depth) {
  int len = euler_bypass.size();
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

int GetLCA(int first, int second, std::vector<std::vector<std::pair<int, int>>>& sparse_table, std::vector<int>& first_entry) {
  int left = first_entry[first];
  int right = first_entry[second];
  if (left > right) {
    std::swap(left, right);
  }
  int max_deg = MyLogDown(right - left + 1);
  int answer = 0;
  if (sparse_table[max_deg][left].first < sparse_table[max_deg][right - pow(2, max_deg) + 1].first) {
    answer = sparse_table[max_deg][left].second;
  } else {
    answer = sparse_table[max_deg][right - pow(2, max_deg) + 1].second;
  }
  return answer;
}

int main() {
  int vertexes_amount;
  std::cin >> vertexes_amount;
  int edges_amount;
  std::cin >> edges_amount;
  int start;
  std::cin >> start;
  --start;
  std::vector<std::vector<Edge>> adjacency_matrix(vertexes_amount);
  for (int i = 1; i <= edges_amount; ++i) {
    int first_vert, second_vert;
    std::cin >> first_vert >> second_vert;
    --first_vert;
    --second_vert;
    adjacency_matrix[first_vert].emplace_back(first_vert, second_vert, i);
    adjacency_matrix[second_vert].emplace_back(second_vert, first_vert, i);
  }
  int timer = 0;
  std::vector<bool> is_visited(vertexes_amount, false);
  std::vector<int> t_in(vertexes_amount);
  std::vector<int> t_up(vertexes_amount);
  FillTUp(adjacency_matrix, start, -1, timer, t_in, t_up, is_visited);
  std::unordered_map<int, bool> is_bridge;
  for (int i = 0; i < vertexes_amount; ++i) {
    for (const Edge& edge : adjacency_matrix[i]) {
      if (t_up[edge.target] > t_in[edge.start]) {
        is_bridge[edge.edge_number] = true;
      }
    }
  }
  std::vector<int> bridges_amount_upper(vertexes_amount);
  bridges_amount_upper[start] = 0;
  std::vector<bool> visited(vertexes_amount, false);
  std::vector<int> parent(vertexes_amount);
  parent[start] = start;
  std::vector<int> depth(vertexes_amount);
  std::vector<int> euler_bypass;
  BuildTree(adjacency_matrix, start, bridges_amount_upper, is_bridge, visited, parent, depth, euler_bypass, 1);
  std::vector<std::vector<std::pair<int, int>>> sparse_table = BuildSparseTable(euler_bypass, depth);
  std::vector<int> first_entry(vertexes_amount, -1);
  for (size_t i = 0; i < euler_bypass.size(); ++i) {
    if (first_entry[euler_bypass[i]] == -1) {
      first_entry[euler_bypass[i]] = i;
    }
  }
  int questions_amount;
  std::cin >> questions_amount;
  for (int i = 0; i < questions_amount; ++i) {
    int first, second;
    std::cin >> first >> second;
    --first;
    --second;
    int lca = GetLCA(first, second, sparse_table, first_entry);
    std::cout << bridges_amount_upper[lca] << "\n";
  }
}