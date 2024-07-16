#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

const int kMax = 1e9;

std::vector<int> GetNeighbours(int position, int height, int width) {
  std::vector<int> answer;
  if (position > width) {
    answer.push_back(position - width);
  }
  if (position <= (height - 1) * width) {
    answer.push_back(position + width);
  }
  if (position % width != 1 && width != 1) {
    answer.push_back(position - 1);
  }
  if (position % width != 0 && width != 1) {
    answer.push_back(position + 1);
  }
  return answer;
}

void dfs(int vertex, std::vector<bool>& visited, int height, int width, std::vector<bool>& is_deleted,
         std::vector<bool>& is_can_be_deleted) {
  visited[vertex] = true;
  for (auto neighbour: GetNeighbours(vertex, height, width)) {
    if (!visited[neighbour] && !is_deleted[neighbour] && !is_can_be_deleted[neighbour]) {
      dfs(neighbour, visited, height, width, is_deleted, is_can_be_deleted);
    }
  }
}

void bfs(int start, int target, std::vector<std::unordered_map<int, int>>& adjacent, std::vector<bool>& visited,
         std::vector<int>& parent) {
  std::queue<int> queue;
  queue.push(start);
  visited[start] = true;
  while (!queue.empty()) {
    int cur_vert = queue.front();
    queue.pop();
    if (cur_vert == target) {
      return;
    }
    for (auto it = adjacent[cur_vert].begin(); it != adjacent[cur_vert].end(); ++it) {
      if (!visited[it->first] && it->second != 0) {
        queue.push(it->first);
        visited[it->first] = true;
        parent[it->first] = cur_vert;
      }
    }
  }
}

void rebuild_graph(int start, int target, std::vector<std::unordered_map<int, int>>& adjacent, std::vector<int>& parent) {
  int cur_vertex = target;
  int smallest_edge = kMax + 1;
  std::vector<std::pair<int, int>> path;
  while (cur_vertex != start) {
    int cur_parent = parent[cur_vertex];
    smallest_edge = std::min(smallest_edge, adjacent[cur_parent][cur_vertex]);
    path.emplace_back(cur_parent, cur_vertex);
    cur_vertex = cur_parent;
  }
  for (size_t i = 0; i < path.size(); ++i) {
    int start = path[i].first;
    int target = path[i].second;
    adjacent[start][target] -= smallest_edge;
    adjacent[target][start] += smallest_edge;
  }
}

void dfs_2(int cur_vertex, std::vector<std::unordered_map<int, int>>& adjacent, std::vector<bool>& visited,
           std::vector<bool>& first_vert_visited, std::vector<bool>& second_vert_visited,
           std::vector<bool>& is_can_be_deleted) {
  visited[cur_vertex] = true;
  if (is_can_be_deleted[cur_vertex / 2]) {
    if (cur_vertex % 2 == 0) {
      first_vert_visited[cur_vertex / 2] = true;
    } else {
      second_vert_visited[cur_vertex / 2] = true;
    }
  }
  for (auto it = adjacent[cur_vertex].begin(); it != adjacent[cur_vertex].end(); ++it) {
    if (!visited[it->first] && it->second != 0) {
      dfs_2(it->first, adjacent, visited, first_vert_visited, second_vert_visited, is_can_be_deleted);
    }
  }
}

int main() {
  int height, width;
  std::cin >> height >> width;
  int deleted_amount, can_be_deleted_amount;
  std::cin >> deleted_amount >> can_be_deleted_amount;
  std::vector<bool> is_deleted(height * width + 1, false);
  for (int i = 0; i < deleted_amount; ++i) {
    int h, w;
    std::cin >> h >> w;
    --h;
    is_deleted[w + h * width] = true;
  }
  std::vector<bool> is_can_be_deleted(height * width + 1, false);
  for (int i = 0; i < can_be_deleted_amount; ++i) {
    int h, w;
    std::cin >> h >> w;
    --h;
    is_can_be_deleted[w + h * width] = true;
  }
  int r_loc;
  int t_loc;
  int r_h, r_w;
  std::cin >> r_h >> r_w;
  --r_h;
  r_loc = r_w + r_h * width;
  int t_h, t_w;
  std::cin >> t_h >> t_w;
  --t_h;
  t_loc = t_w + t_h * width;
  std::vector<bool> visited(height * width + 1, false);
  dfs(r_loc, visited, height, width, is_deleted, is_can_be_deleted);
  if (visited[t_loc]) {
    std::cout << "-1";
    return 0;
  }
  std::vector<std::unordered_map<int, int>> adjacent(height * width * 2 + 2);
  for (int i = 0; i < height; ++i) {
    for (int j = 1; j <= width; ++j) {
      int cur_vert = j + i * width;
      if (is_deleted[cur_vert]) {
        continue;
      }
      if (is_can_be_deleted[cur_vert]) {
        adjacent[cur_vert * 2][cur_vert * 2 + 1] = 1;
        adjacent[cur_vert * 2 + 1][cur_vert * 2] = 0;
      } else {
        adjacent[cur_vert * 2][cur_vert * 2 + 1] = kMax;
        adjacent[cur_vert * 2 + 1][cur_vert * 2] = 0;
      }
      for (int neighbour : GetNeighbours(cur_vert, height, width)) {
        if (is_deleted[neighbour]) {
          continue;
        }
        adjacent[neighbour * 2 + 1][cur_vert * 2] = kMax;
        adjacent[cur_vert * 2][neighbour * 2 + 1] = 0;
        adjacent[cur_vert * 2 + 1][neighbour * 2] = kMax;
        adjacent[neighbour * 2][cur_vert * 2 + 1] = 0;
      }
    }
  }
  int start = r_loc * 2 + 1;
  int finish = t_loc * 2;
  std::vector<bool> is_visited(height * width * 2 + 2, false);
  std::vector<int> parent(height * width * 2 + 2);
  while (true) {
    bfs(start, finish, adjacent, is_visited, parent);
    if (!is_visited[finish]) {
      break;
    }
    rebuild_graph(start, finish, adjacent, parent);
    is_visited.assign(height * width * 2 + 2, false);
  }
  std::vector<bool> first_vert_visited(height * width + 1, false);
  std::vector<bool> second_vert_visited(height * width + 1, false);
  is_visited.assign(height * width * 2 + 2, false);
  dfs_2(start, adjacent, is_visited, first_vert_visited, second_vert_visited, is_can_be_deleted);
  std::vector<std::pair<int, int>> answer;
  for (int i = 1; i <= height * width; ++i) {
    if (first_vert_visited[i] && !second_vert_visited[i]) {
      int row = (i - 1) / width + 1;
      int column = (i - 1) % width + 1;
      answer.emplace_back(row, column);
    }
  }
  std::cout << answer.size() << "\n";
  for (size_t i = 0; i < answer.size(); ++i) {
    std::cout << answer[i].first << ' ' << answer[i].second << "\n";
  }
}