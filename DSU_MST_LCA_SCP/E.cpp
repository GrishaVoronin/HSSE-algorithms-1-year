#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>

struct Edge{
  Edge(int64_t start, int64_t target, int64_t weight) : start(start), target(target), weight(weight) {}
  int64_t start;
  int64_t target;
  int64_t weight;
  bool operator==(const Edge& other) {
    return start == other.start && target == other.target && weight == other.weight;
  }
};

struct cmp {
  bool operator() (const Edge& first, const Edge& second) const {
    if (first.weight == second.weight) {
      return first.target > second.target;
    }
    return first.weight > second.weight;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int64_t vertexes_amount;
  std::cin >> vertexes_amount;
  int64_t edges_amount;
  std::cin >> edges_amount;
  int64_t pairs_amount;
  std::cin >> pairs_amount;
  std::vector<std::vector<Edge>> adjacency_matrix(vertexes_amount);
  for (int64_t i = 0; i < edges_amount; ++i) {
    int64_t start, finish, weight;
    std::cin >> start >> finish >> weight;
    --start;
    --finish;
    adjacency_matrix[start].emplace_back(start, finish, weight);
    adjacency_matrix[finish].emplace_back(finish, start, weight);
  }
  std::vector<int64_t> depth(vertexes_amount);
  std::vector<int64_t> distance(vertexes_amount, 0);
  std::set<Edge, cmp> queue_for_mst;
  std::unordered_map<int64_t, bool> is_used;
  int64_t vertexes_used = 0;
  std::vector<int64_t> parent(vertexes_amount);
  queue_for_mst.insert({0, 0, 0});
  parent[0] = 0;
  depth[0] = 1;
  while (vertexes_used != vertexes_amount) {
    if (vertexes_used == 5) {
      int b = 0;
    }
    Edge cur_edge = *queue_for_mst.begin();
    queue_for_mst.erase(queue_for_mst.begin());
    int64_t cur_vert = cur_edge.target;
    is_used[cur_vert] = true;
    ++vertexes_used;
    for (const auto edge : adjacency_matrix[cur_vert]) {
      if (is_used.contains(edge.target)) {
        continue;
      }
      if (edge.weight > distance[edge.target]) {
        if (queue_for_mst.contains({parent[edge.target], edge.target, distance[edge.target]})) {
          queue_for_mst.erase({parent[edge.target], edge.target, distance[edge.target]});
        }
        parent[edge.target] = cur_vert;
        distance[edge.target] = edge.weight;
        depth[edge.target] = depth[cur_vert] + 1;
        queue_for_mst.insert(edge);
      }
    }
  }
  for (int64_t i = 0; i < pairs_amount; ++i) {
    int64_t start, finish;
    std::cin >> start >> finish;
    --start;
    --finish;
    if (depth[start] < depth[finish]) {
      std::swap(start, finish);
    }
    int64_t depth_difference = depth[start] - depth[finish];
    int64_t answer = 1e9;
    for (int64_t j = 0; j < depth_difference; ++j) {
      answer = std::min(answer, distance[start]);
      start = parent[start];
    }
    while (start != finish) {
      answer = std::min(answer, distance[start]);
      start = parent[start];
      answer = std::min(answer, distance[finish]);
      finish = parent[finish];
    }
    std::cout << answer << "\n";
  }
}