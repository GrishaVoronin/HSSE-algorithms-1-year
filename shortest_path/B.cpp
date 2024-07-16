#include <iostream>
#include <vector>
#include <set>

int main() {
  const long long kMaxDistance = 1e18;
  int vertexes = 0;
  std::cin >> vertexes;
  int edges = 0;
  std::cin >> edges;
  int viruses_amount = 0;
  std::cin >> viruses_amount;
  std::vector<int> viruses;
  for (int i = 0; i < viruses_amount; ++i) {
    int virus_vert = 0;
    std::cin >> virus_vert;
    --virus_vert;
    viruses.push_back(virus_vert);
  }
  std::vector<std::vector<std::pair<int, long long>>> adjacency(vertexes);
  for (int i = 0; i < edges; ++i) {
    int start = 0;
    std::cin >> start;
    --start;
    int finish = 0;
    std::cin >> finish;
    --finish;
    int weight = 0;
    std::cin >> weight;
    adjacency[start].push_back(std::make_pair(finish, weight));
    adjacency[finish].push_back(std::make_pair(start, weight));
  }
  std::vector<long long> vert_closed(vertexes, kMaxDistance);
  std::set<std::pair<long long, int>> viruses_distance;
  for (int i = 0; i < viruses_amount; ++i) {
    viruses_distance.insert({0, viruses[i]});
    vert_closed[viruses[i]] = 0;
  }
  while (!viruses_distance.empty()) {
    long long min_distance = viruses_distance.begin()->first;
    if (min_distance == kMaxDistance) {
      break;
    }
    int cur_vert = viruses_distance.begin()->second;
    viruses_distance.erase(viruses_distance.begin());
    vert_closed[cur_vert] = min_distance;
    for (auto [target_vert, weight] : adjacency[cur_vert]) {
      if (vert_closed[target_vert] > min_distance + weight) {
        if (viruses_distance.contains(std::make_pair(vert_closed[target_vert], target_vert))) {
          viruses_distance.erase(std::make_pair(vert_closed[target_vert], target_vert));
        }
        vert_closed[target_vert] = min_distance + weight;
        viruses_distance.insert(std::make_pair(vert_closed[target_vert], target_vert));
      }
    }
  }
  int start = 0;
  std::cin >> start;
  --start;
  int finish = 0;
  std::cin >> finish;
  --finish;
  std::vector<long long> answer(vertexes, kMaxDistance);
  std::set<std::pair<long long, int>> distance;
  distance.insert({0, start});
  while (!distance.empty()) {
    long long min_distance = distance.begin()->first;
    if (min_distance == kMaxDistance) {
      break;
    }
    int cur_vert = distance.begin()->second;
    distance.erase(distance.begin());
    answer[cur_vert] = min_distance;
    for (auto [target_vert, weight] : adjacency[cur_vert]) {
      if (answer[target_vert] > min_distance + weight && min_distance + weight < vert_closed[target_vert]) {
        if (distance.contains(std::make_pair(answer[target_vert], target_vert))) {
          distance.erase(std::make_pair(answer[target_vert], target_vert));
        }
        answer[target_vert] = min_distance + weight;
        distance.insert(std::make_pair(answer[target_vert], target_vert));
      }
    }
  }
  if (answer[finish] == kMaxDistance) {
    std::cout << -1;
  } else {
    std::cout << answer[finish];
  }
}