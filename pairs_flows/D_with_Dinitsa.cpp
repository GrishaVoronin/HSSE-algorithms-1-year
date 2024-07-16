#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

const int kMaxDistance = 1e9;

struct Edge {
  Edge(int start, int target, int capacity, int flow, size_t reverse_edge_number)
  : start(start),
    target(target),
    capacity(capacity),
    flow(flow),
    reverse_edge_number(reverse_edge_number) {}

  int start;
  int target;
  int capacity;
  int flow;
  size_t reverse_edge_number;
};

std::vector<int> bfs(const std::vector<std::vector<Edge>>& adjacent, int start, int target, int cur_flow) {
  std::vector<int> distance(adjacent.size(), kMaxDistance);
  std::queue<int> queue;
  queue.push(start);
  distance[start] = 0;
  while (!queue.empty()) {
    int vertex = queue.front();
    queue.pop();
    if (vertex == target) {
      return distance;
    }
    for (const auto& edge: adjacent[vertex]) {
      if (distance[edge.target] == kMaxDistance && edge.capacity - edge.flow >= cur_flow) {
        queue.push(edge.target);
        distance[edge.target] = distance[edge.start] + 1;
      }
    }
  }
  return distance;
}

int dfs(int vertex, int cur_flow, std::vector<std::vector<Edge>>& adjacent, std::vector<int>& ptr, int target,
        std::vector<int>& distance) {
  if (vertex == target) {
    return cur_flow;
  }
  while (ptr[vertex] < adjacent[vertex].size()) {
    Edge& edge = adjacent[vertex][ptr[vertex]];
    if (distance[vertex] + 1 != distance[edge.target] || edge.capacity - edge.flow < cur_flow) {
      ++ptr[vertex];
      continue;
    }
    int ans = dfs(edge.target, cur_flow, adjacent, ptr, target, distance);
    if (ans == 0) {
      ++ptr[vertex];
      continue;
    }
    edge.flow += ans;
    adjacent[edge.target][edge.reverse_edge_number].flow -= ans;
    return ans;
  }
  return 0;
}

int main() {
  int vertexes_amount;
  std::cin >> vertexes_amount;
  int edges_amount;
  std::cin >> edges_amount;
  std::vector<std::pair<int, size_t>> start_position;
  std::vector<std::vector<Edge>> adjacent(vertexes_amount);
  for (int i = 0; i < edges_amount; ++i) {
    int start;
    std::cin >> start;
    --start;
    int target;
    std::cin >> target;
    --target;
    int capacity;
    std::cin >> capacity;
    size_t edge_position = adjacent[start].size();
    size_t reversed_edge_position = adjacent[target].size();
    adjacent[start].emplace_back(start, target, capacity, 0, reversed_edge_position);
    adjacent[target].emplace_back(target, start, 0, 0, edge_position);
    start_position.emplace_back(start, edge_position);
  }
  int cur_flow = pow(2, 29);
  long long answer = 0;
  while (cur_flow >= 1) {
    std::vector<int> distance = bfs(adjacent, 0, vertexes_amount - 1, cur_flow);
    if (distance[vertexes_amount - 1] == kMaxDistance) {
      cur_flow /= 2;
      continue;
    }
    std::vector<int> ptr(vertexes_amount, 0);
    while (true) {
      int flow = dfs(0, cur_flow, adjacent, ptr, vertexes_amount - 1, distance);
      answer += flow;
      if (flow == 0) {
        distance = bfs(adjacent, 0, vertexes_amount - 1, cur_flow);
        ptr.assign(vertexes_amount, 0);
        if (distance[vertexes_amount - 1] == kMaxDistance) {
          break;
        }
      }
    }
    cur_flow /= 2;
  }
  std::cout << answer << "\n";
  for (int i = 0; i < edges_amount; ++i) {
    std::cout << adjacent[start_position[i].first][start_position[i].second].flow << "\n";
  }
}