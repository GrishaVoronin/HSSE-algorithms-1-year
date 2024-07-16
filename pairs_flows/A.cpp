#include <iostream>
#include <vector>
#include <queue>

void bfs(int start, int vertexes_amount, std::vector<std::vector<int>>& adjacency_matrix, std::vector<bool>& visited,
         std::vector<int>& parent) {
  std::queue<int> queue;
  queue.push(start);
  visited[start] = true;
  while (!queue.empty()) {
    int cur_vert = queue.front();
    queue.pop();
    if (cur_vert == vertexes_amount - 1) {
      return;
    }
    for (int target = 0; target < vertexes_amount; ++target) {
      if (!visited[target] && adjacency_matrix[cur_vert][target] != 0) {
        queue.push(target);
        visited[target] = true;
        parent[target] = cur_vert;
      }
    }
  }
}

void rebuild_graph(int vertexes_amount, std::vector<std::vector<int>>& adjacency_matrix, std::vector<int>& parent,
                   long long& answer) {
  int cur_vertex = vertexes_amount - 1;
  int smallest_edge = 1e6;
  std::vector<std::pair<int, int>> path;
  while (cur_vertex != 0) {
    int cur_parent = parent[cur_vertex];
    smallest_edge = std::min(smallest_edge, adjacency_matrix[cur_parent][cur_vertex]);
    path.emplace_back(cur_parent, cur_vertex);
    cur_vertex = cur_parent;
  }
  answer += smallest_edge;
  for (size_t i = 0; i < path.size(); ++i) {
    int start = path[i].first;
    int target = path[i].second;
    adjacency_matrix[start][target] -= smallest_edge;
    adjacency_matrix[target][start] += smallest_edge;
  }
}

int main() {
  int vertexes_amount;
  std::cin >> vertexes_amount;
  int edges_amount;
  std::cin >> edges_amount;
  std::vector<std::vector<int>> adjacency_matrix(vertexes_amount, std::vector<int>(vertexes_amount, 0));
  for (int i = 0; i < edges_amount; ++i) {
    int start;
    std::cin >> start;
    --start;
    int finish;
    std::cin >> finish;
    --finish;
    int weight;
    std::cin >> weight;
    adjacency_matrix[start][finish] = weight;
  }
  std::vector<bool> visited(vertexes_amount, false);
  std::vector<int> parent(vertexes_amount);
  long long answer = 0;
  while (true) {
    bfs(0, vertexes_amount, adjacency_matrix, visited, parent);
    if (!visited[vertexes_amount - 1]) {
      break;
    }
    rebuild_graph(vertexes_amount, adjacency_matrix, parent, answer);
    visited.assign(vertexes_amount, false);
  }
  std::cout << answer;
}