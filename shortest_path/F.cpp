#include <iostream>
#include <vector>

const long long kMaxDistance = 1e18;

void GetPath(int start, int finish, std::vector<std::vector<int>>& mid_vert, std::vector<int>& path) {
  while (start != finish) {
    path.push_back(start);
    start = mid_vert[start][finish];
  }
}

int main() {
  int vertexes = 0;
  std::cin >> vertexes;
  int edges = 0;
  std::cin >> edges;
  int lectures_amount = 0;
  std::cin >> lectures_amount;
  std::vector<std::vector<std::pair<int, long long>>> adjacency(vertexes);
  std::vector<std::vector<int>> edge_number(vertexes, std::vector<int>(vertexes));
  for (int i = 0; i < edges; ++i) {
    int start = 0;
    std::cin >> start;
    --start;
    int finish = 0;
    std::cin >> finish;
    --finish;
    //edge_number[start][finish] = i + 1;
    long long weight = 0;
    std::cin >> weight;
    weight *= -1;
    bool flag = false;
    size_t position = 0;
    for (size_t j = 0; j < adjacency[start].size(); ++j) {
      if (adjacency[start][j].first == finish) {
        flag = true;
        position = j;
      }
    }
    if (flag) {
      if (adjacency[start][position].second > weight) {
        adjacency[start][position].second = weight;
        edge_number[start][finish] = i + 1;
      }
    } else {
      adjacency[start].push_back({finish, weight});
      edge_number[start][finish] = i + 1;
    }
  }
  std::vector<int> lectures;
  for (int i = 0; i < lectures_amount; ++i) {
    int lec = 0;
    std::cin >> lec;
    --lec;
    lectures.push_back(lec);
  }
  std::vector<std::vector<long long>> dp(vertexes, std::vector<long long>(vertexes, kMaxDistance));
  std::vector<std::vector<int>> mid_vert(vertexes, std::vector<int>(vertexes, -1));
  for (int i = 0; i < vertexes; ++i) {
    dp[i][i] = 0;
    mid_vert[i][i] = i;
  }
  for (int i = 0; i < vertexes; ++i) {
    for (auto [target_vert, weight] : adjacency[i]) {
      dp[i][target_vert] = std::min(dp[i][target_vert], weight);
      mid_vert[i][target_vert] = target_vert;
    }
  }
  for (int k = 0; k < vertexes; ++k) {
    for (int start_vert = 0; start_vert < vertexes; ++start_vert) {
      for (int finish_vert = 0; finish_vert < vertexes; ++finish_vert) {
        if (dp[start_vert][k] == kMaxDistance || dp[k][finish_vert] == kMaxDistance) {
          continue;
        }
        if (dp[start_vert][k] + dp[k][finish_vert] < dp[start_vert][finish_vert]) {
          dp[start_vert][finish_vert] = dp[start_vert][k] + dp[k][finish_vert];
          mid_vert[start_vert][finish_vert] = mid_vert[start_vert][k];
        }
      }
    }
  }
  std::vector<int> vertexes_in_cycle;
  for (int i = 0; i < vertexes; ++i) {
    if (dp[i][i] < 0) {
      vertexes_in_cycle.push_back(i);
    }
  }
  for (int i = 0; i < lectures_amount - 1; ++i) {
    for (size_t j = 0; j < vertexes_in_cycle.size(); ++j) {
      int cycle_vert = vertexes_in_cycle[j];
      if (dp[lectures[i]][cycle_vert] != kMaxDistance && dp[cycle_vert][lectures[i + 1]] != kMaxDistance) {
        std::cout << "infinitely kind" << "\n";
        return 0;
      }
    }
  }
  std::vector<int> path;
  for (int i = 0; i < lectures_amount - 1; ++i) {
    if (lectures[i] != lectures[i + 1]) {
      GetPath(lectures[i], lectures[i + 1], mid_vert, path);
    }
  }
  path.push_back(lectures[lectures_amount - 1]);
  std::cout << path.size() - 1 << "\n";
  for (size_t i = 0; i < path.size() - 1; ++i) {
    std::cout << edge_number[path[i]][path[i + 1]] << ' ';
  }
  std::cout << "\n";
}