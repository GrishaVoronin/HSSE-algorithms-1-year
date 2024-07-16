#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  const int kMaxDistance = 100000;
  int vertexes = 0;
  std::cin >> vertexes;
  std::vector<std::vector<std::pair<int, int>>> adjacency(vertexes);
  for (int i = 0; i < vertexes; ++i) {
    for (int j = 0; j < vertexes; ++j) {
      int weight = 0;
      std::cin >> weight;
      if (weight != kMaxDistance) {
        adjacency[i].push_back(std::make_pair(j, weight));
      }
    }
  }
  std::vector<bool> visited(vertexes, false);
  for (size_t i = 0; i < vertexes; ++i) {
    if (!visited[vertexes]) {
      std::vector<int> dp(vertexes, kMaxDistance);
      std::vector<int> parent(vertexes, -1);
      dp[i] = 0;
      visited[i] = true;
      for (int k = 1; k <= vertexes - 1; ++k) {
        for (int start_vert = 0; start_vert < vertexes; ++start_vert) {
          if (dp[start_vert] == kMaxDistance) {
            continue;
          }
          for (auto [target_vert, weight] : adjacency[start_vert]) {
            if (dp[target_vert] > dp[start_vert] + weight) {
              visited[target_vert] = true;
              dp[target_vert] = dp[start_vert] + weight;
              parent[target_vert] = start_vert;
            }
          }
        }
      }
      for (int start_vert = 0; start_vert < vertexes; ++start_vert) {
        if (dp[start_vert] == kMaxDistance) {
          continue;
        }
        for (auto [target_vert, weight] : adjacency[start_vert]) {
          if (dp[target_vert] > dp[start_vert] + weight) {
            std::cout << "YES" << "\n";
            parent[target_vert] = start_vert;
            for (int i = 0; i < vertexes; ++i) {
              target_vert = parent[target_vert];
            }
            std::vector<int> answer;
            int cycle_start = target_vert;
            answer.push_back(cycle_start);
            while (cycle_start != parent[target_vert]) {
              target_vert = parent[target_vert];
              answer.push_back(target_vert);
            }
            answer.push_back(cycle_start);
            std::cout << answer.size() << "\n";
            for (int i = answer.size() - 1; i >= 0; --i) {
              std::cout << answer[i] + 1 << ' ';
            }
            return 0;
          }
        }
      }
    }
  }
  std::cout << "NO";
}