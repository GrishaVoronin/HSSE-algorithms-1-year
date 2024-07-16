#include <iostream>
#include <vector>
#include <set>

int main() {
  const int kMaxDistance = 2009000999;
  int graph_amount = 0;
  std::cin >> graph_amount;
  for (int i = 0; i < graph_amount; ++i) {
    int vertexes = 0;
    std::cin >> vertexes;
    int edges = 0;
    std::cin >> edges;
    std::vector<std::vector<std::pair<int, int>>> adjacency(vertexes);
    for (int j = 0; j < edges; ++j) {
      int start = 0;
      std::cin >> start;
      int finish = 0;
      std::cin >> finish;
      int weight = 0;
      std::cin >> weight;
      adjacency[start].push_back(std::make_pair(finish, weight));
      adjacency[finish].push_back(std::make_pair(start, weight));
    }
    int start_vertex = 0;
    std::cin >> start_vertex;
    std::vector<int> answer(vertexes, kMaxDistance);
    std::set<std::pair<int, int>> distance;
    distance.insert({0, start_vertex});
    for (int j = 0; j < vertexes; ++j) {
      if (j != start_vertex) {
        distance.insert({kMaxDistance, j});
      }
    }
    while (!distance.empty()) {
      int min_distance = distance.begin()->first;
      if (min_distance == kMaxDistance) {
        break;
      }
      int cur_vert = distance.begin()->second;
      distance.erase(distance.begin());
      answer[cur_vert] = min_distance;
      for (auto [target_vert, weight] : adjacency[cur_vert]) {
        if (answer[target_vert] > min_distance + weight) {
          distance.erase(std::make_pair(answer[target_vert], target_vert));
          answer[target_vert] = min_distance + weight;
          distance.insert(std::make_pair(answer[target_vert], target_vert));
        }
      }
    }
    for (int j = 0; j < vertexes; ++j) {
      std::cout << answer[j] << " ";
    }
    std::cout << "\n";
  }
}