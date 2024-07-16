#include <iostream>
#include <vector>
#include <algorithm>

void dfs_for_vertexes_order(std::vector<std::vector<int>>& adjacency_matrix, std::vector<bool>& visited,
         std::vector<int>& out_order, int vert) {
  visited[vert] = true;
  for (int neighbour: adjacency_matrix[vert]) {
    if (!visited[neighbour]) {
      dfs_for_vertexes_order(adjacency_matrix, visited, out_order, neighbour);
    }
  }
  out_order.push_back(vert);
}

void dfs_for_components(std::vector<std::vector<int>>& reversed_adjacency_matrix, std::vector<bool>& visited,
                        int vert, int cur_component, std::vector<int>& answer) {
  visited[vert] = true;
  answer[vert - 1] = cur_component;
  for (int neighbour: reversed_adjacency_matrix[vert]) {
    if (!visited[neighbour]) {
      dfs_for_components(reversed_adjacency_matrix, visited, neighbour, cur_component, answer);
    }
  }
}

int main() {
  int vertexes = 0;
  std::cin >> vertexes;
  int edges = 0;
  std::cin >> edges;
  std::vector<std::vector<int>> adjacency_matrix(vertexes + 1);
  std::vector<std::vector<int>> reversed_adjacency_matrix(vertexes + 1);
  for (int i = 0; i < edges; ++i) {
    int start = 0;
    std::cin >> start;
    int finish = 0;
    std::cin >> finish;
    adjacency_matrix[start].push_back(finish);
    reversed_adjacency_matrix[finish].push_back(start);
  }
  std::vector<bool> visited(vertexes + 1, false);
  std::vector<int> out_order;
  for (int i = 1; i <= vertexes; ++i) {
    if (!visited[i]) {
      dfs_for_vertexes_order(adjacency_matrix, visited, out_order, i);
    }
  }
  std::vector<int> answer(vertexes, 0);
  std::reverse(out_order.begin(), out_order.end());
  int cur_component = 0;
  visited = std::vector<bool>(vertexes + 1, false);
  for (int vert: out_order) {
    if (!visited[vert]) {
      ++cur_component;
      dfs_for_components(reversed_adjacency_matrix, visited, vert, cur_component, answer);
    }
  }
  std::cout << cur_component << "\n";
  for (int i = 0; i < vertexes; ++i) {
    std::cout << answer[i] << " ";
  }
}