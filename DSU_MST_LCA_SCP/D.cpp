#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

int main() {
  const long long kMaxDistance = 1e18;
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int vertexes_amount = 0;
  std::cin >> vertexes_amount;
  std::vector<std::vector<std::pair<int, int>>> adjacency_matrix(vertexes_amount);
  for (int i = 0; i < vertexes_amount; ++i) {
    for (int j = 0; j < vertexes_amount; ++j) {
      long long weight;
      std::cin >> weight;
      if (i != j) {
        adjacency_matrix[i].push_back({weight, j});
      }
    }
  }
  std::vector<long long> distance(vertexes_amount, kMaxDistance);
  std::set<std::pair<long long, int>> queue_for_mst;
  std::unordered_map<int, bool> is_used;
  long long mst_weight = 0;
  int vertexes_used = 0;
  for (int i = 0; i < vertexes_amount; ++i) {
    long long cur_vert_cost;
    std::cin >> cur_vert_cost;
    queue_for_mst.insert({cur_vert_cost, i});
    distance[i] = cur_vert_cost;
  }
  while (vertexes_used != vertexes_amount) {
    int cur_vertex = queue_for_mst.begin()->second;
    long long weight = queue_for_mst.begin()->first;
    queue_for_mst.erase(queue_for_mst.begin());
    is_used[cur_vertex] = true;
    mst_weight += weight;
    ++vertexes_used;
    for (const auto edge: adjacency_matrix[cur_vertex]) {
      if (is_used.contains(edge.second)) {
        continue;
      }
      if (edge.first < distance[edge.second]) {
        queue_for_mst.erase({distance[edge.second], edge.second});
        distance[edge.second] = edge.first;
        queue_for_mst.insert(edge);
      }
    }
  }
  std::cout << mst_weight;
}