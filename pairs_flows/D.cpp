#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

struct Edge {
  Edge(int start, int finish, int weight) : start(start), target(finish), weight(weight) {}
  int start;
  int target;
  int weight;
};

void bfs(int start, int vertexes_amount, std::vector<std::vector<int>>& adjacency_matrix, std::vector<bool>& visited,
         std::vector<int>& parent, int cur_flow) {
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
      if (!visited[target] && adjacency_matrix[cur_vert][target] >= cur_flow) {
        queue.push(target);
        visited[target] = true;
        parent[target] = cur_vert;
      }
    }
  }
}

void rebuild_graph(int vertexes_amount, std::vector<std::vector<int>>& adjacency_matrix, std::vector<int>& parent,
                   std::vector<std::vector<int>>& flow, int cur_flow) {
  int cur_vertex = vertexes_amount - 1;
  std::vector<std::pair<int, int>> path;
  while (cur_vertex != 0) {
    int cur_parent = parent[cur_vertex];
    path.emplace_back(cur_parent, cur_vertex);
    cur_vertex = cur_parent;
  }
  for (size_t i = 0; i < path.size(); ++i) {
    int start = path[i].first;
    int target = path[i].second;
    adjacency_matrix[start][target] -= cur_flow;
    adjacency_matrix[target][start] += cur_flow;
    flow[start][target] += cur_flow;
    flow[target][start] -= cur_flow;
  }
}

int main() {
  int vertexes_amount;
  std::cin >> vertexes_amount;
  int edges_amount;
  std::cin >> edges_amount;
  std::vector<Edge> edges;
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
    adjacency_matrix[start][finish] += weight;
    edges.emplace_back(start, finish, weight);
  }
  std::vector<std::vector<int>> flow(vertexes_amount, std::vector<int>(vertexes_amount, 0));
  int cur_flow = pow(2, 29);
  long long answer = 0;
  std::vector<bool> visited(false, vertexes_amount);
  std::vector<int> parent(vertexes_amount);
  while (cur_flow >= 1) {
    while (true) {
      visited.assign(vertexes_amount, false);
      bfs(0, vertexes_amount, adjacency_matrix, visited, parent, cur_flow);
      if (!visited[vertexes_amount - 1]) {
        break;
      }
      answer+=cur_flow;
      rebuild_graph(vertexes_amount, adjacency_matrix, parent, flow, cur_flow);
    }
    cur_flow /= 2;
  }
  std::cout << answer << "\n";
  for (int i = 0; i < edges_amount; ++i) {
    int start = edges[i].start;
    int target = edges[i].target;
    int weight = edges[i].weight;
    if (flow[start][target] >= weight) {
      std::cout << weight << "\n";
      flow[start][target] -= weight;
    } else {
      std::cout << std::max(0, flow[start][target]) << "\n";
      flow[start][target] = 0;
    }
  }
}