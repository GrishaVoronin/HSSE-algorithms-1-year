#include <iostream>
#include <vector>

class Vertex {
 public:
  Vertex() = default;
  Vertex(int id) :
      vertex_id(id)
  {}

  void AddNeighbour(int neighbour) {
    neighbours.push_back(neighbour);
  }
  std::vector<int>& GetNeighbours() {
    return neighbours;
  }

  int GetVertexId() {
    return vertex_id;
  }

  bool visited = false;
  int time_in = 0;
  int time_up = 0;
  int children = 0;

 private:
  std::vector<int> neighbours;
  int vertex_id;
};

class Graph {
 public:
  Graph(int vertexes_amount) :
      vertexes_amount(vertexes_amount) {
    for (int i = 0; i <= vertexes_amount; ++i) {
      vertexes.push_back(Vertex(i));
    }
  }

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

 private:
  int vertexes_amount = 0;
  std::vector<Vertex> vertexes;
};

void FillTUp(Graph& graph, Vertex& from, Vertex& ancestor, int& timer, std::vector<bool>& answer,
             int start_vert) {
  from.visited = true;
  ++timer;
  from.time_in = timer;
  from.time_up = timer;
  for (int& neighbour_id : from.GetNeighbours()) {
    Vertex& to = graph.GetVertexById(neighbour_id);
    if (ancestor.GetVertexId() == neighbour_id) {
      continue;
    }
    if (to.visited) {
      from.time_up = std::min(from.time_up, to.time_in);
    } else {
      FillTUp(graph, to, from, timer, answer, start_vert);
      ++from.children;
      from.time_up = std::min(from.time_up, to.time_up);
    }
  }
  if (from.time_up >= ancestor.time_in && ancestor.GetVertexId() != start_vert) {
    answer[ancestor.GetVertexId()] = true;
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

  std::vector<bool> answer(vertexes + 1, false);
  int timer = 0;
  for (int i = 1; i <= vertexes; ++i) {
    Vertex& vert = graph.GetVertexById(i);
    if (!vert.visited) {
      Vertex zero_vert = Vertex(-1);
      zero_vert.time_in = 200000 + 1;
      FillTUp(graph, vert, zero_vert, timer, answer, i);
      if (vert.children > 1) {
        answer[i] = true;
      }
    }
  }
  int cnt = 0;
  for (int i = 1; i <= vertexes; ++i) {
    if (answer[i]) {
      ++cnt;
    }
  }
  std::cout << cnt << "\n";
  for (int i = 1; i <= vertexes; ++i) {
    if (answer[i]) {
      std::cout << i << "\n";
    }
  }
}