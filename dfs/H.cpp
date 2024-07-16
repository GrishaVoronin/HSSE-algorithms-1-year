#include <iostream>
#include <vector>
#include <unordered_set>

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
  bool viseted_for_dfs_2 = false;
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

void dfs(Graph& graph, Vertex& from, Vertex& ancestor, int& timer, int& leafs,
         std::unordered_set<int>& bridges_vertexes, int& first_ancestor, int& first_child) {
  from.visited = true;
  ++timer;
  from.time_in = timer;
  from.time_up = timer;
  bool ancestor_visited = false;
  int leafs_before_dfs = leafs;
  for (int neighbour_id : from.GetNeighbours()) {
    Vertex &to = graph.GetVertexById(neighbour_id);
    if (ancestor.GetVertexId() == neighbour_id and !ancestor_visited) {
      ancestor_visited = true;
      continue;
    }
    if (to.visited) {
      from.time_up = std::min(from.time_up, to.time_in);
    } else {
      dfs(graph, to, from, timer, leafs, bridges_vertexes, first_ancestor, first_child);
      from.time_up = std::min(from.time_up, to.time_up);
    }
  }
  if (ancestor.time_in < from.time_up) {
    first_ancestor = ancestor.GetVertexId();
    first_child = from.GetVertexId();
    bridges_vertexes.insert(ancestor.GetVertexId());
    bridges_vertexes.insert(from.GetVertexId());
    if (leafs_before_dfs == leafs) {
      ++leafs;
    }
  }
}

void dfs_2(Vertex& vert, Graph& graph, int& first_ancestor, int& first_child,
           std::unordered_set<int>& bridges_vertexes, bool& flag) {
  if (flag) {
    return;
  }
  vert.viseted_for_dfs_2 = true;
  for (int neighbour_id : vert.GetNeighbours()) {
    if (neighbour_id == first_child or neighbour_id == first_ancestor) {
      continue;
    }
    if (bridges_vertexes.contains(neighbour_id)) {
      flag = true;
      break;
    }
    Vertex& neighbour = graph.GetVertexById(neighbour_id);
    if (!neighbour.viseted_for_dfs_2) {
      dfs_2(neighbour, graph, first_ancestor, first_child, bridges_vertexes, flag);
    }
  }
}


int main() {
  std::cin.tie(nullptr);
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
    if (start != finish) {
      graph.AddEdge(start, finish);
    }
    edges[i] = {start, finish};
  }
  int timer = 0;
  int leafs = 0;
  int first_ancestor = 0;
  int first_child = 0;
  std::unordered_set<int> bridges_vertexes;
  for (int i = 1; i <= vertexes; ++i) {
    Vertex& vert = graph.GetVertexById(i);
    if (!vert.visited) {
      Vertex zero_vert = Vertex(-1);
      zero_vert.time_in = 5000 + 1;
      dfs(graph, vert, zero_vert, timer, leafs, bridges_vertexes, first_ancestor, first_child);
    }
  }
  if (bridges_vertexes.empty()) {
    std::cout << 0;
  } else {
    bool flag = false;
    Vertex& start_vert = graph.GetVertexById(first_ancestor);
    dfs_2(start_vert, graph, first_ancestor, first_child, bridges_vertexes, flag);
    if (!flag) {
      ++leafs;
    }
    std::cout << (leafs + 1)/2;
  }
}