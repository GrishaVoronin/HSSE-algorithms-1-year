#include <iostream>
#include <vector>

int main() {
  const int kMaxDistance = 30000;
  int vertexes = 0;
  std::cin >> vertexes;
  int edges = 0;
  std::cin >> edges;
  std::vector<std::vector<std::pair<int, int>>> adjacency(vertexes);
  for (int j = 0; j < edges; ++j) {
    int start = 0;
    std::cin >> start;
    --start;
    int finish = 0;
    std::cin >> finish;
    --finish;
    int weight = 0;
    std::cin >> weight;
    adjacency[start].push_back(std::make_pair(finish, weight));
  }
  std::vector<int> prev_layer(vertexes, kMaxDistance);
  prev_layer[0] = 0;
  for (int k = 1; k <= vertexes - 1; ++k) {
    std::vector<int> cur_layer = prev_layer;
    for (int start_vert = 0; start_vert < vertexes; ++start_vert) {
      if (prev_layer[start_vert] == kMaxDistance) {
        continue;
      }
      for (auto [target_vert, weight] : adjacency[start_vert]) {
        if (cur_layer[target_vert] > prev_layer[start_vert] + weight) {
          cur_layer[target_vert] = prev_layer[start_vert] + weight;
        }
      }
    }
    prev_layer = cur_layer;
  }
  for (int i = 0; i < vertexes; ++i) {
    std::cout << prev_layer[i] << ' ';
  }
}
