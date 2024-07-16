#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

int main() {
  int vertexes_amount = 0;
  std::cin >> vertexes_amount;
  int edges_amount = 0;
  std::cin >> edges_amount;
  std::vector<std::vector<std::pair<int, int>>> adjacency_matrix(vertexes_amount);
  for (int i = 0; i < edges_amount; ++i) {
    int start, finish;
    long long weight;
    std::cin >> start >> finish >> weight;
    --start;
    --finish;
    adjacency_matrix[start].push_back({weight, finish});
    adjacency_matrix[finish].push_back({weight, start});
  }
  std::set<std::pair<long long, int>> queue_for_mst;
  std::unordered_map<int, bool> is_used;
  long long mst_weight = 0;
  int vertexes_used = 0;
  queue_for_mst.insert({0, 0});
  while (vertexes_used != vertexes_amount) {
    int cur_vertex = queue_for_mst.begin()->second;
    long long weight = queue_for_mst.begin()->first;
    queue_for_mst.erase(queue_for_mst.begin());
    if (is_used.contains(cur_vertex)) {
      continue;
    }
    is_used[cur_vertex] = true;
    mst_weight += weight;
    ++vertexes_used;
    for (const auto edge: adjacency_matrix[cur_vertex]) {
      queue_for_mst.insert(edge);
    }
  }
  std::cout << mst_weight;
}