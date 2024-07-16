#include <iostream>
#include <vector>
#include <set>

struct Edge {
  int target_vert;
  long long time;
};

int main() {
  const int vertexes_amount = 1e6 + 500;
  int final_vertex = 0;
  std::cin >> final_vertex;
  --final_vertex;
  long long up_cost = 0;
  std::cin >> up_cost;
  long long down_cost = 0;
  std::cin >> down_cost;
  long long entrance_cost = 0;
  std::cin >> entrance_cost;
  long long exit_cost = 0;
  std::cin >> exit_cost;
  int teleports_amount = 0;
  std::cin >> teleports_amount;
  std::vector<std::vector<Edge>> adjacency (vertexes_amount);
  for (int i = 0; i <= 1e6 - 2; ++i) {
    adjacency[i].push_back({i + 1, up_cost});
  }
  for (int i = 1; i <= 1e6 - 1; ++i) {
    adjacency[i].push_back({i - 1, down_cost});
  }
  for (int i = 0; i < teleports_amount; ++i) {
    int k_teleport_amount = 0;
    std::cin >> k_teleport_amount;
    for (int j = 0; j < k_teleport_amount; ++j) {
      int connected_vert = 0;
      std::cin >> connected_vert;
      --connected_vert;
      adjacency[connected_vert].push_back({static_cast<int>(1e6 + i), entrance_cost});
      adjacency[static_cast<int>(1e6 + i)].push_back({connected_vert, exit_cost});
    }
  }
  const long long kMaxDistance = 1e18;
  std::vector<long long> distance(vertexes_amount, kMaxDistance);
  std::set<std::pair<long long, int>> set;
  distance[0] = 0;
  set.insert({0, 0});
  while (!set.empty()) {
    long long min_distance = set.begin()->first;
    if (min_distance == kMaxDistance) {
      break;
    }
    int cur_vert = set.begin()->second;
    set.erase(set.begin());
    distance[cur_vert] = min_distance;
    for (auto [target_vert, weight] : adjacency[cur_vert]) {
      if (distance[target_vert] > min_distance + weight) {
        set.erase({distance[target_vert], target_vert});
        distance[target_vert] = min_distance + weight;
        set.insert({distance[target_vert], target_vert});
      }
    }
  }
  std::cout << distance[final_vertex];
}