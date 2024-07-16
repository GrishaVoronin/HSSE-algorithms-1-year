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

void FillTUp(Graph& graph, Vertex& from, Vertex& ancestor, int& timer) {
  from.visited = true;
  ++timer;
  from.time_in = timer;
  from.time_up = timer;
  bool ancestor_visited = false;
  for (int& neighbour_id : from.GetNeighbours()) {
    Vertex& to = graph.GetVertexById(neighbour_id);
    if (ancestor.GetVertexId() == neighbour_id and !ancestor_visited) {
      ancestor_visited = true;
      continue;
    }
    if (to.visited) {
      from.time_up = std::min(from.time_up, to.time_in);
    } else {
      FillTUp(graph, to, from, timer);
      from.time_up = std::min(from.time_up, to.time_up);
    }
  }
}

int main() {
  int vertexes = 0;
  std::cin >> vertexes;
  int edges_amount = 0;
  std::cin >> edges_amount;
  Graph graph(vertexes);
  std::vector<std::pair<int, int>> edges(edges_amount);

  for (int i = 0; i < edges_amount; ++i) {
    int start = 0;
    std::cin >> start;
    int finish = 0;
    std::cin >> finish;
    graph.AddEdge(start, finish);
    edges[i] = {start, finish};
  }
  int timer = 0;
  for (int i = 1; i <= vertexes; ++i) {
    Vertex& vert = graph.GetVertexById(i);
    if (!vert.visited) {
      Vertex zero_vert = Vertex(-1);
      FillTUp(graph, vert, zero_vert, timer);
    }
  }
  std::vector<int> answer;
  for (int i = 0; i < edges_amount; ++i) {
    int time_up_start = graph.GetVertexById(edges[i].first).time_up;
    int time_in_start = graph.GetVertexById(edges[i].first).time_in;
    int time_up_finish = graph.GetVertexById(edges[i].second).time_up;
    int time_in_finish = graph.GetVertexById(edges[i].second).time_in;
    if (time_in_start < time_up_finish or time_in_finish < time_up_start) {
      answer.push_back(i + 1);
    }
  }
  std::cout << answer.size() << "\n";
  for (auto& i: answer) {
    std::cout << i << " ";
  }
}