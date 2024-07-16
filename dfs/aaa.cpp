#include <iostream>
#include <unordered_map>
#include <vector>

enum class Colors {
  KWhite = 0,
  KGray = 1,
  KBlack = 2,
};

template <typename Vertex>
class UnorientedUnweightedEdge {
 public:
  UnorientedUnweightedEdge(const Vertex& from, const Vertex& to) :
      vertices_(from, to) {}

  const Vertex& GetStart() const {
    return vertices_.first;
  }

  const Vertex& GetTarget() const {
    return vertices_.second;
  }

  UnorientedUnweightedEdge ReversedEdge() const {
    return UnorientedUnweightedEdge(vertices_.second, vertices_.first);
  }

  bool is_weighted = false;
  bool is_oriented = false;

 private:
  std::pair<Vertex, Vertex> vertices_;
};

template <typename Vertex, typename Edge>
class Graph {
 public:
  using VertexT = Vertex;
  using EdgeT = Edge;

  Graph(int vertexes_amount, int edges_amount)
      : vertexes_amount_(vertexes_amount),
        edges_amount_(edges_amount) {}

  void AddEdge(const EdgeT& edge) {
    adjacent_[edge.GetStart()].push_back(edge);
    if (!edge.is_oriented) {
      EdgeT reversed_edge = edge.ReversedEdge();
      adjacent_[reversed_edge.GetStart()].push_back(reversed_edge);
    }
  }

  const std::vector<EdgeT>& GetOutgoingEdges(const Vertex& vertex) {
    return adjacent_[vertex];
  }

  int GetVertexesAmount() {
    return vertexes_amount_;
  }

  int GetEdgesAmount() {
    return edges_amount_;
  }

 private:
  std::unordered_map<VertexT , std::vector<EdgeT>> adjacent_;
  int vertexes_amount_ = 0;
  int edges_amount_ = 0;
};

template <class Graph>
class AbstractDFSVisitor {
 public:
  virtual void DiscoverVertex(const typename Graph::VertexT& vertex) = 0;
  virtual void FinishVertex(const typename Graph::VertexT& vertex) = 0;
  virtual Colors GetColor(const typename Graph::VertexT& vertex) = 0;
  virtual ~AbstractDFSVisitor() = default;

 protected:
  std::unordered_map<typename Graph::VertexT, Colors> colors_;
};

template <class Graph>
class ComponentsDFSVisitor : public AbstractDFSVisitor<Graph> {
 public:
  void DiscoverVertex(const typename Graph::VertexT& vertex) final {
    BaseClass::colors_[vertex] = Colors::KGray;
    component_number[vertex] = current_component_number;
  }

  void FinishVertex(const typename Graph::VertexT& vertex) final {
    BaseClass::colors_[vertex] = Colors::KBlack;
  }

  Colors GetColor(const typename Graph::VertexT& vertex) final {
    if (BaseClass::colors_.contains(vertex)) {
      return BaseClass::colors_[vertex];
    }
    return Colors::KWhite;
  }

  virtual ~ComponentsDFSVisitor() = default;

  std::unordered_map<typename Graph::VertexT, int> GetComponentNumber() {
    return component_number;
  }

  void UpdateCurrentComponentNumber() {
    ++current_component_number;
  }

  int GetMaxComponent() {
    return current_component_number;
  }

 private:
  using BaseClass = AbstractDFSVisitor<Graph>;
  std::unordered_map<typename Graph::VertexT, int> component_number;
  int current_component_number = 0;
};

template <class Graph, class Visitor>
void DFS(Graph& graph, const typename Graph::VertexT& vertex, Visitor& visitor) {
  visitor.DiscoverVertex(vertex);
  for (const auto& outgoing_edge : graph.GetOutgoingEdges(vertex)) {
    const auto& neighbour = outgoing_edge.GetTarget();
    if (visitor.GetColor(neighbour) == Colors::KWhite) {
      DFS(graph, neighbour, visitor);
    }
  }
  visitor.FinishVertex(vertex);
}

using MyGraph = Graph<int, UnorientedUnweightedEdge<int>>;

MyGraph CreateGraph(int vertexes_amount, int edges_amount) {
  MyGraph graph(vertexes_amount, edges_amount);
  for (int i = 0; i < edges_amount; ++i) {
    int start = 0;
    std::cin >> start;
    int finish = 0;
    std::cin >> finish;
    UnorientedUnweightedEdge<int> edge(start, finish);
    graph.AddEdge(edge);
  }
  return graph;
}

void PrintAnswer(int vertexes_amount, ComponentsDFSVisitor<MyGraph>& visitor) {
  int max_component = visitor.GetMaxComponent();
  std::unordered_map<typename MyGraph::VertexT, int> component_number = visitor.GetComponentNumber();
  std::cout << max_component << "\n";
  std::vector<std::vector<MyGraph::VertexT>> components_vertexes(max_component + 1);
  for (int i = 1; i <= vertexes_amount; ++i) {
    components_vertexes[component_number[i]].push_back(i);
  }
  for (int i = 1; i <= max_component; ++i) {
    std::cout << components_vertexes[i].size() << "\n";
    for (auto id : components_vertexes[i]) {
      std::cout << id << " ";
    }
    std::cout << "\n";
  }
}

int main() {
  int vertexes_amount = 0;
  std::cin >> vertexes_amount;
  int edges_amount = 0;
  std::cin >> edges_amount;
  MyGraph graph = CreateGraph(vertexes_amount, edges_amount);
  ComponentsDFSVisitor<MyGraph> visitor;
  for (int i = 1; i <= vertexes_amount; ++i) {
    if (visitor.GetColor(i) == Colors::KWhite) {
      visitor.UpdateCurrentComponentNumber();
      DFS(graph, i, visitor);
    }
  }
  PrintAnswer(vertexes_amount, visitor);
}