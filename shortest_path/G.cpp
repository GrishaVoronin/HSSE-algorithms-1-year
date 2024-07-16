#include <iostream>
#include <vector>

int main() {
  int vertexes = 0;
  std::cin >> vertexes;
  std::vector<std::vector<int>> dp(vertexes, std::vector<int>(vertexes));
  for (int i = 0; i < vertexes; ++i) {
    for (int j = 0; j < vertexes; ++j) {
      std::cin >> dp[i][j];
    }
  }
  for (int k = 0; k < vertexes; ++k) {
    for (int start_vert = 0; start_vert < vertexes; ++start_vert) {
      for (int finish_vert = 0; finish_vert < vertexes; ++finish_vert) {
        if (dp[start_vert][k] * dp[k][finish_vert] == 1) {
          dp[start_vert][finish_vert] = 1;
        }
      }
    }
  }
  for (int start_vert = 0; start_vert < vertexes; ++start_vert) {
    for (int finish_vert = 0; finish_vert < vertexes; ++finish_vert) {
      std::cout << dp[start_vert][finish_vert] << ' ';
    }
    std::cout << "\n";
  }
}