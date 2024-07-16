#include <iostream>
#include <vector>
#include <algorithm>

class Vertex {
 public:
  Vertex() = default;
  Vertex(int id) :
      vertex_id(id)
  {}

  void AddNeighbour(int neighbour) {
    neighbours.push_back(neighbour);
  }
  std::vector<int> GetNeighbours() {
    return neighbours;
  }

  int GetVertexId() {
    return vertex_id;
  }

  bool visited = false;

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
  }

  Vertex& GetVertexById (int id) {
    return vertexes[id];
  }

  void AddOutVertex(int vert_id) {
    out_time.push_back(vert_id);
  }

  std::vector<int> GetOutTimeVertexes() {
    return out_time;
  }

 private:
  int vertexes_amount = 0;
  std::vector<Vertex> vertexes;
  std::vector<int> out_time;
};

void dfs_for_vertexes_out_order(Vertex& vert, Graph& graph) {
  vert.visited = true;
  for (int neighbour_id: vert.GetNeighbours()) {
    Vertex& neighbour = graph.GetVertexById(neighbour_id);
    if (!neighbour.visited) {
      dfs_for_vertexes_out_order(neighbour, graph);
    }
  }
  graph.AddOutVertex(vert.GetVertexId());
}

void dfs_for_components(int vert_id, Graph& graph, int cur_component, std::vector<int>& answer) {
  Vertex& vert = graph.GetVertexById(vert_id);
  vert.visited = true;
  answer[vert_id] = cur_component;
  for (int neighbour_id: vert.GetNeighbours()) {
    Vertex& neighbour = graph.GetVertexById(neighbour_id);
    if (!neighbour.visited) {
      dfs_for_components(neighbour_id, graph, cur_component, answer);
    }
  }
}

int main() {
  int vertexes = 0;
  std::cin >> vertexes;
  int edges = 0;
  std::cin >> edges;
  Graph graph(vertexes);
  Graph reversed_graph(vertexes);

  for (int i = 0; i < edges; ++i) {
    int start = 0;
    std::cin >> start;
    int finish = 0;
    std::cin >> finish;
    graph.AddEdge(start, finish);
    reversed_graph.AddEdge(finish, start);
  }
  for (int i = 1; i <= vertexes; ++i) {
    Vertex& vert = graph.GetVertexById(i);
    if (!vert.visited) {
      dfs_for_vertexes_out_order(vert, graph);
    }
  }
  std::vector<int> answer(vertexes + 1, 0);
  std::vector<int> out_order = graph.GetOutTimeVertexes();
  std::reverse(out_order.begin(), out_order.end());
  int cur_component = 0;
  for (int vert_id: out_order) {
    if (!reversed_graph.GetVertexById(vert_id).visited) {
      ++cur_component;
      dfs_for_components(vert_id, reversed_graph, cur_component, answer);
    }
  }
  std::cout << cur_component << "\n";
  for (int i = 1; i <= vertexes; ++i) {
    std::cout << answer[i] << " ";
  }
}
