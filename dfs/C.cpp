#include <iostream>
#include <vector>

bool has_cycle = false;

void dfs_for_cycle(int vert, std::vector<char>& colors, std::vector<std::vector<int>>& adjacency_matrix) {
  colors[vert] = 'g';
  for (int neighbour: adjacency_matrix[vert]) {
    if (colors[neighbour] == 'w') {
      dfs_for_cycle(neighbour, colors, adjacency_matrix);
    } else if (colors[neighbour] == 'g') {
      if (!has_cycle) {
        has_cycle = true;
      }
    }
  }
  colors[vert] = 'b';
}

bool have_cycle(int vertexes, std::vector<std::vector<int>>& adjacency_matrix) {
  std::vector<char> colors(vertexes + 1, 'w');
  for (int i = 1; i <= vertexes; ++i) {
    if (colors[i] == 'w') {
      dfs_for_cycle(i, colors, adjacency_matrix);
    }
  }
  return has_cycle;
}


void dfs(std::vector<std::vector<int>>& adjacency_matrix, std::vector<int>& vertexes_out,
         std::vector<bool>& visited, int vert) {
  visited[vert] = true;
  for (int neighbour: adjacency_matrix[vert]) {
    if (!visited[neighbour]) {
      dfs(adjacency_matrix, vertexes_out, visited, neighbour);
    }
  }
  vertexes_out.push_back(vert);
}

int main() {
  int vertexes = 0;
  std::cin >> vertexes;
  int edges = 0;
  std::cin >> edges;
  std::vector<std::vector<int>> adjacency_matrix(vertexes + 1);
  for (int i = 0; i < edges; ++i) {
    int start = 0;
    std::cin >> start;
    int finish = 0;
    std::cin >> finish;
    adjacency_matrix[start].push_back(finish);
  }
  std::vector<int> vertexes_out;
  std::vector<bool> visited(vertexes + 1, false);
  if (have_cycle(vertexes, adjacency_matrix)) {
    std::cout << -1;
  } else {
    for (int i = 1; i <= vertexes; ++i) {
      if (!visited[i]) {
        dfs(adjacency_matrix, vertexes_out, visited, i);
      }
    }
    for (int i = vertexes_out.size() - 1; i >= 0; --i) {
      std::cout << vertexes_out[i] << " ";
    }
  }
}