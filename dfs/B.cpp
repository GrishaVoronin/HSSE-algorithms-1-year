#include <iostream>
#include <vector>

void print_answer(std::vector<int>& clon_squad_number, int max_squad_number) {
  std::cout << max_squad_number << "\n";
  std::vector<std::vector<int>> clons_id(max_squad_number + 1); // clons_id[i] - clons from i squad
  for (size_t i = 1; i < clon_squad_number.size(); ++i) {
    clons_id[clon_squad_number[i]].push_back(i);
  }
  for (size_t i = 1; i <= max_squad_number; ++i) {
    std::cout << clons_id[i].size() << "\n";
    for (auto id: clons_id[i]) {
      std::cout << id << " ";
    }
    std::cout << "\n";
  }
}

void dfs(std::vector<std::vector<int>>& adjacency_matrix, std::vector<int>& clon_squad_number,
         int vert, int cur_squad_num) {
  clon_squad_number[vert] = cur_squad_num;
  for (int neighbour: adjacency_matrix[vert]) {
    if (clon_squad_number[neighbour] == 0) {
      dfs(adjacency_matrix, clon_squad_number, neighbour, cur_squad_num);
    }
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
    adjacency_matrix[finish].push_back(start);
  }
  std::vector<int> clon_squad_number(vertexes + 1, 0); // 0 if still not in the squad
  int cur_squad_num = 0;
  for (int i = 1; i <= vertexes; ++i) {
    if (clon_squad_number[i] == 0) {
      ++cur_squad_num;
      dfs(adjacency_matrix, clon_squad_number, i, cur_squad_num);
    }
  }
  print_answer(clon_squad_number, cur_squad_num);
}
