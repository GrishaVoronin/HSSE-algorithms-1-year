#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>


enum class Colors {
  KWhite = 0,
  KGray = 1,
  KBlack = 2,
};

template <typename Vertex>
class OrientedUnweightedEdge {
 public:
  OrientedUnweightedEdge(const Vertex& from, const Vertex& to) :
      vertices_(from, to) {}

  const Vertex& GetStart() const {
    return vertices_.first;
  }

  const Vertex& GetTarget() const {
    return vertices_.second;
  }

  OrientedUnweightedEdge ReversedEdge() const {
    return OrientedUnweightedEdge(vertices_.second, vertices_.first);
  }

  bool is_weighted = false;
  bool is_oriented = true;

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
class OutOrderDFSVisitor : public AbstractDFSVisitor<Graph> {
 public:
  void DiscoverVertex(const typename Graph::VertexT& vertex) final {
    BaseClass::colors_[vertex] = Colors::KGray;
  }

  void FinishVertex(const typename Graph::VertexT& vertex) final {
    BaseClass::colors_[vertex] = Colors::KBlack;
    vertexes_out_order.push_back(vertex);
  }

  Colors GetColor(const typename Graph::VertexT& vertex) final {
    if (BaseClass::colors_.contains(vertex)) {
      return BaseClass::colors_[vertex];
    }
    return Colors::KWhite;
  }

  std::vector<typename Graph::VertexT> GetOutOrder() {
    return vertexes_out_order;
  };

  virtual ~OutOrderDFSVisitor() = default;

 private:
  using BaseClass = AbstractDFSVisitor<Graph>;
  std::vector<typename Graph::VertexT> vertexes_out_order;
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

using MyGraph = Graph<int, OrientedUnweightedEdge<int>>;

void CreateGraphs(int edges_amount, MyGraph& graph, MyGraph& reversed_graph) {
  for (int i = 0; i < edges_amount; ++i) {
    int start = 0;
    std::cin >> start;
    int finish = 0;
    std::cin >> finish;
    OrientedUnweightedEdge<int> edge(start, finish);
    graph.AddEdge(edge);
    OrientedUnweightedEdge<int> reversed_edge(finish, start);
    reversed_graph.AddEdge(reversed_edge);
  }
}

void PrintAnswer(int vertexes_amount, ComponentsDFSVisitor<MyGraph>& visitor) {
  std::cout << visitor.GetMaxComponent() << "\n";
  std::unordered_map<MyGraph::VertexT, int> vertexes_components = visitor.GetComponentNumber();
  for (int i = 1; i <= vertexes_amount; ++i) {
    std::cout << vertexes_components[i] << ' ';
  }
}

int main() {
  int vertexes_amount = 0;
  std::cin >> vertexes_amount;
  int edges_amount = 0;
  std::cin >> edges_amount;
  MyGraph graph(vertexes_amount, edges_amount);
  MyGraph reversed_graph(vertexes_amount, edges_amount);
  CreateGraphs(edges_amount, graph, reversed_graph);
  OutOrderDFSVisitor<MyGraph> out_order_dfs_visitor;
  for (int i = 1; i <= vertexes_amount; ++i) {
    if (out_order_dfs_visitor.GetColor(i) == Colors::KWhite) {
      DFS(graph, i, out_order_dfs_visitor);
    }
  }
  std::vector<MyGraph::VertexT> out_order = out_order_dfs_visitor.GetOutOrder();
  std::reverse(out_order.begin(), out_order.end());
  ComponentsDFSVisitor<MyGraph> components_dfs_visitor;
  for (auto i : out_order) {
    if (components_dfs_visitor.GetColor(i) == Colors::KWhite) {
      components_dfs_visitor.UpdateCurrentComponentNumber();
      DFS(reversed_graph, i, components_dfs_visitor);
    }
  }
  PrintAnswer(vertexes_amount, components_dfs_visitor);
}