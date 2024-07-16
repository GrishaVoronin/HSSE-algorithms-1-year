#include <iostream>
#include <vector>

class Vertex {
 public:
  Vertex() = default;
  void AddNeighbour(int neighbour) {
    neighbours.push_back(neighbour);
  }
  std::vector<int> GetNeighbours() {
    return neighbours;
  }
  int squad_number_ = 0;

 private:
  std::vector<int> neighbours;
};

class Graph {
 public:
  Graph(int vertexes_amount) :
    vertexes_amount(vertexes_amount), vertexes(std::vector<Vertex>(vertexes_amount + 1, Vertex()))
    {}

  int Size() {
    return vertexes_amount;
  }

  void AddEdge(int start, int finish) {
    vertexes[start].AddNeighbour(finish);
    vertexes[finish].AddNeighbour(start);
  }

  Vertex& GetVertexById (int id) {
    return vertexes[id];
  }

  void PrintAnswer(int max_squad_number) {
    std::cout << max_squad_number << "\n";
    std::vector<std::vector<int>> clons_id(max_squad_number + 1); // clons_id[i] - clons from i squad
    for (size_t i = 1; i <= Size(); ++i) {
      Vertex vert = GetVertexById(i);
      clons_id[vert.squad_number_].push_back(i);
    }
    for (size_t i = 1; i <= max_squad_number; ++i) {
      std::cout << clons_id[i].size() << "\n";
      for (auto id: clons_id[i]) {
        std::cout << id << " ";
      }
      std::cout << "\n";
    }
  }

 private:
  int vertexes_amount = 0;
  std::vector<Vertex> vertexes;
};

void dfs(Vertex& vert, Graph& graph, int cur_squad_num) {
  vert.squad_number_ = cur_squad_num;
  for (int neighbour_id: vert.GetNeighbours()) {
    Vertex& neighbour = graph.GetVertexById(neighbour_id);
    if (neighbour.squad_number_ == 0) {
      dfs(neighbour, graph, cur_squad_num);
    }
  }
}

int main() {
  int vertexes = 0;
  std::cin >> vertexes;
  int edges = 0;
  std::cin >> edges;
  Graph graph(vertexes);
  for (int i = 0; i < edges; ++i) {
    int start = 0;
    std::cin >> start;
    int finish = 0;
    std::cin >> finish;
    graph.AddEdge(start, finish);
  }

  int cur_squad_num = 0;
  for (size_t i = 1; i <= graph.Size(); ++i) {
    Vertex& vert = graph.GetVertexById(i);
    if (vert.squad_number_ == 0) {
      ++cur_squad_num;
      dfs(vert, graph, cur_squad_num);
    }
  }
  graph.PrintAnswer(cur_squad_num);
}