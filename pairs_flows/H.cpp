#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

template <typename Vertex>
class OrientedUnweightedEdge {
 public:
  OrientedUnweightedEdge(const Vertex& from, const Vertex& to)
      : start_(from), target_(to) {}

  const Vertex& GetStart() const { return start_; }

  const Vertex& GetTarget() const { return target_; }

  bool is_weighted = false;
  bool is_oriented = true;

 private:
  Vertex start_;
  Vertex target_;
};

template <typename Vertex, typename Edge>
class BipartiteGraph {
 public:
  using VertexT = Vertex;
  using EdgeT = Edge;

  BipartiteGraph(int left_size, int right_size)
      : left_size_(left_size), right_size_(right_size) {}

  void AddEdgeFromLeft(const EdgeT& edge) {
    edges_from_left_part_[edge.GetStart()].push_back(edge);
    vertexes_from_left_.insert(edge.GetStart());
    vertexes_from_right_.insert(edge.GetTarget());
  }

  void AddEdgeFromRight(const EdgeT& edge) {
    edges_from_right_part_[edge.GetStart()].push_back(edge);
    vertexes_from_left_.insert(edge.GetTarget());
    vertexes_from_right_.insert(edge.GetStart());
  }

  const std::vector<EdgeT>& GetOutgoingEdgesFromLeft(const Vertex& vertex) const {
    return edges_from_left_part_.at(vertex);
  }

  const std::vector<EdgeT>& GetOutgoingEdgesFromRight(const Vertex& vertex) const {
    return edges_from_right_part_.at(vertex);
  }

  bool augment(const VertexT& vertex, std::unordered_map<VertexT, VertexT>& matching, std::unordered_map<VertexT, bool>& is_used) const {
    if (is_used.contains(vertex)) {
      return false;
    }
    is_used[vertex] = true;
    for (const auto& edge : GetOutgoingEdgesFromLeft(vertex)) {
      const VertexT& neighbour = edge.GetTarget();
      if (!matching.contains(neighbour) || augment(matching[neighbour], matching, is_used)) {
        matching[neighbour] = vertex;
        return true;
      }
    }
    return false;
  }

  std::unordered_map<VertexT , VertexT> GetMatching() const {
    std::unordered_map<VertexT, VertexT> matching;
    std::unordered_map<VertexT, bool> is_used;
    for (const auto& vertex: vertexes_from_left_) {
      if (augment(vertex, matching, is_used)) {
        is_used.clear();
      }
    }
    return matching;
  }

 private:
  int left_size_ = 0;
  int right_size_ = 0;
  std::set<VertexT> vertexes_from_left_;
  std::set<VertexT> vertexes_from_right_;
  std::unordered_map<VertexT, std::vector<EdgeT>> edges_from_left_part_;
  std::unordered_map<VertexT, std::vector<EdgeT>> edges_from_right_part_;
};

std::vector<int> GetNeighbours(int position, int height, int width) {
  std::vector<int> answer;
  if (position > width) {
    answer.push_back(position - width);
  }
  if (position <= (height - 1) * width) {
    answer.push_back(position + width);
  }
  if (position % width != 1) {
    answer.push_back(position - 1);
  }
  if (position % width != 0) {
    answer.push_back(position + 1);
  }
  return answer;
}

void Solve() {
  using MyGraph = BipartiteGraph<int, OrientedUnweightedEdge<int>>;
  int height, width;
  std::cin >> height >> width;
  int cost_2x1, cost_1x1;
  std::cin >> cost_2x1 >> cost_1x1;
  std::vector<int> white;
  std::unordered_map<int, bool> black;
  for (int i = 0; i < height; ++i) {
    for (int j = 1; j <= width; ++j) {
      char cell;
      std::cin >> cell;
      if (cell == '*') {
        if ((i + j) % 2 == 1) {
          white.push_back(i * width + j);
        } else {
          black[i * width + j] = true;
        }
      }
    }
  }
  if (cost_1x1 * 2 <= cost_2x1) {
    std::cout << cost_1x1 * static_cast<int>(white.size() + black.size());
  } else {
    MyGraph graph(white.size(), black.size());
    for (int start: white) {
      for (int finish: GetNeighbours(start, height, width)) {
        if (black.contains(finish)) {
          graph.AddEdgeFromLeft({start, finish});
        }
      }
    }
    std::unordered_map<int, int> matching = graph.GetMatching();
    int amount_2x1 = static_cast<int>(matching.size());
    std::cout << amount_2x1 * cost_2x1 + (static_cast<int>(white.size() + black.size()) - 2 * amount_2x1) * cost_1x1;
  }
}

int main() { Solve(); }