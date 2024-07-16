#include <iostream>
#include <vector>
#include <stack>

bool have_cycle = false;

void print_cycle(std::stack<int>& vertexes_order, int vertex) {
  if (vertexes_order.top() != vertex) {
    int vert_to_print = vertexes_order.top();
    vertexes_order.pop();
    print_cycle(vertexes_order, vertex);
    std::cout << vert_to_print << " ";
  }
}

void dfs(int vert, std::vector<char>& colors, std::stack<int>& vertexes_order, std::vector<std::vector<int>>& adjacency_matrix) {
  vertexes_order.push(vert);
  colors[vert] = 'g';
  for (int neighbour: adjacency_matrix[vert]) {
    if (colors[neighbour] == 'w') {
      dfs(neighbour, colors, vertexes_order, adjacency_matrix);
    } else if (colors[neighbour] == 'g') {
      if (!have_cycle) {
        have_cycle = true;
        std::cout << "YES" << "\n";
        std::cout << neighbour << " ";
        print_cycle(vertexes_order, neighbour);
      }
    }
  }
  colors[vert] = 'b';
  if (!vertexes_order.empty()) {
    vertexes_order.pop();
  }
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
  std::vector<char> colors(vertexes + 1, 'w');
  std::stack<int> vertexes_order;
  for (int i = 1; i <= vertexes; ++i) {
    if (have_cycle) {
      return 0;
    }
    if (colors[i] == 'w') {
      dfs(i, colors, vertexes_order, adjacency_matrix);
    }
  }
  if (!have_cycle) {
    std::cout << "NO";
  }
}