#include <iostream>
#include <vector>
#include <set>

struct Vertex {
  Vertex(long long vertex_number, long long time, long long cost) : vertex_number(vertex_number), time(time), cost(cost) {}
  Vertex(const Vertex& vertex) = default;
  long long vertex_number;
  long long time;
  long long cost;
};

struct Cmp {
  bool operator () (const Vertex& vertex_1, const Vertex& vertex_2) const {
    return vertex_1.time <= vertex_2.time;
  }
};

struct vertex_hash {
  size_t operator()(const Vertex& vertex) const {
    return std::hash<long long>()(vertex.vertex_number) ^ std::hash<long long>()(vertex.time);
  }
};

int main() {
  const long long kMaxDistance = 1e18;
  long long vertexes_amount = 0;
  std::cin >> vertexes_amount;
  long long edges_amount = 0;
  std::cin >> edges_amount;
  long long time_limit = 0;
  std::cin >> time_limit;
  std::vector<std::vector<Vertex>> adjacency(vertexes_amount);
  for (long long i = 0; i < edges_amount; ++i) {
    long long start = 0;
    std::cin >> start;
    --start;
    long long finish = 0;
    std::cin >> finish;
    --finish;
    long long cost = 0;
    std::cin >> cost;
    long long time = 0;
    std::cin >> time;
    adjacency[start].emplace_back(finish, time, cost);
    adjacency[finish].emplace_back(start, time, cost);
  }
  std::set<Vertex, Cmp> set;
  Vertex start(0, 0, 0);
  std::vector<std::vector<Vertex>> parent(vertexes_amount, std::vector<Vertex>(time_limit + 1, start));
  std::vector<long long> distance(vertexes_amount, kMaxDistance);
  std::vector<long long> time_to_vert(vertexes_amount, kMaxDistance);
  distance[0] = 0;
  time_to_vert[0] = 0;
  set.insert({0, 0, 0});
  while (!set.empty()) {
    Vertex cur_vert = *set.begin();
    set.erase(set.begin());
    for (auto& [target_vert, time, cost] : adjacency[cur_vert.vertex_number]) {
      if (cur_vert.time + time > time_limit) {
        continue;
      } else {
        if (cur_vert.cost + cost < distance[target_vert]) {
          set.erase({target_vert, time_to_vert[target_vert], distance[target_vert]});
          distance[target_vert] = cur_vert.cost + cost;
          set.insert({target_vert, cur_vert.time + time, cur_vert.cost + cost});
          time_to_vert[target_vert] = cur_vert.time + time;
          parent[target_vert][cur_vert.time + time] = cur_vert;
        }
      }
    }
  }
  if (distance[vertexes_amount - 1] == kMaxDistance) {
    std::cout << -1;
  } else {
    if (vertexes_amount == 1) {
      std::cout << 0 << "\n";
      std::cout << 0 << "\n";
      std::cout << 1 << "\n";
    } else {
      long long final_cost = distance[vertexes_amount - 1];
      std::vector<long long> path;
      Vertex last_vertex = Vertex(vertexes_amount - 1, time_to_vert[vertexes_amount - 1], distance[vertexes_amount - 1]);
      while (last_vertex.vertex_number != 0) {
        path.push_back(last_vertex.vertex_number);
        last_vertex = parent[last_vertex.vertex_number][last_vertex.time];
      }
      std::cout << final_cost << "\n";
      std::cout << path.size() + 1 << "\n";
      std::cout << 1 << ' ';
      for (int i = path.size() - 1; i >= 0; --i) {
        std::cout << path[i] + 1 << ' ';
      }
    }
  }
}