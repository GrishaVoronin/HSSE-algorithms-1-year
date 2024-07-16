#include <iostream>
#include <vector>
#include <queue>

const int64_t kMaxDistance = 1e18;

void bfs(int64_t start, int64_t vertexes_amount, std::vector<std::vector<int64_t>>& adjacency_matrix, std::vector<bool>& visited,
         std::vector<int64_t>& parent) {
  std::queue<int64_t> queue;
  queue.push(start);
  visited[start] = true;
  while (!queue.empty()) {
    int64_t cur_vert = queue.front();
    queue.pop();
    if (cur_vert == vertexes_amount - 1) {
      return;
    }
    for (int64_t target = 0; target < vertexes_amount; ++target) {
      if (!visited[target] && adjacency_matrix[cur_vert][target] != 0) {
        queue.push(target);
        visited[target] = true;
        parent[target] = cur_vert;
      }
    }
  }
}

void rebuild_graph(int64_t vertexes_amount, std::vector<std::vector<int64_t>>& adjacency_matrix, std::vector<int64_t>& parent,
                   long long& answer) {
  int64_t cur_vertex = vertexes_amount - 1;
  int64_t smallest_edge = 1e6;
  std::vector<std::pair<int64_t, int64_t>> path;
  while (cur_vertex != 0) {
    int64_t cur_parent = parent[cur_vertex];
    smallest_edge = std::min(smallest_edge, adjacency_matrix[cur_parent][cur_vertex]);
    path.emplace_back(cur_parent, cur_vertex);
    cur_vertex = cur_parent;
  }
  answer += smallest_edge;
  for (size_t i = 0; i < path.size(); ++i) {
    int64_t start = path[i].first;
    int64_t target = path[i].second;
    adjacency_matrix[start][target] -= smallest_edge;
    adjacency_matrix[target][start] += smallest_edge;
  }
}

int main() {
  int64_t teams_amount;
  std::cin >> teams_amount;
  std::vector<int64_t> victory_amount(teams_amount);
  for (int64_t i = 0; i < teams_amount; ++i) {
    std::cin >> victory_amount[i];
  }
  std::vector<int64_t> games_amount(teams_amount);
  for (int64_t i = 0; i < teams_amount; ++i) {
    std::cin >> games_amount[i];
  }
  std::vector<std::vector<int64_t>> games(teams_amount, std::vector<int64_t>(teams_amount));
  for (int64_t i = 0; i < teams_amount; ++i) {
    for (int64_t j = 0; j < teams_amount; ++j) {
      std::cin >> games[i][j];
    }
  }
  int64_t my_team_max_wins = victory_amount[0] + games_amount[0];
  for (int64_t i = 1; i < teams_amount; ++i) {
    if (victory_amount[i] > my_team_max_wins) {
      std::cout << "NO";
      return 0;
    }
  }
  int64_t games_in_graph = 0;
  int64_t total_points = 0;
  for (int64_t i = 1; i < teams_amount; ++i) {
    for (int64_t j = i + 1; j < teams_amount; ++j) {
      if (games[i][j] != 0) {
        ++games_in_graph;
        total_points += games[i][j];
      }
    }
  }
  int64_t vertexes_amount = teams_amount + games_in_graph + 1;
  std::vector<std::vector<int64_t>> adjacency_matrix(vertexes_amount, std::vector<int64_t>(vertexes_amount, 0));
  for (int64_t i = 1; i < teams_amount; ++i) {
    adjacency_matrix[0][i] = my_team_max_wins - victory_amount[i];
  }
  int64_t cur_game_vert = teams_amount;
  for (int64_t i = 1; i < teams_amount; ++i) {
    for (int64_t j = i + 1; j < teams_amount; ++j) {
      if (games[i][j] != 0) {
        adjacency_matrix[i][cur_game_vert] = kMaxDistance;
        adjacency_matrix[j][cur_game_vert] = kMaxDistance;
        adjacency_matrix[cur_game_vert][vertexes_amount - 1] = games[i][j];
        ++cur_game_vert;
      }
    }
  }
  std::vector<bool> visited(vertexes_amount, false);
  std::vector<int64_t> parent(vertexes_amount);
  long long answer = 0;
  while (true) {
    bfs(0, vertexes_amount, adjacency_matrix, visited, parent);
    if (!visited[vertexes_amount - 1]) {
      break;
    }
    rebuild_graph(vertexes_amount, adjacency_matrix, parent, answer);
    visited.assign(vertexes_amount, false);
  }
  if (answer == total_points) {
    std::cout << "YES";
  } else {
    std::cout << "NO";
  }
}