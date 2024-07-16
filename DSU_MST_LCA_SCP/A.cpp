#include <iostream>
#include <vector>

class DSU{
 public:
  DSU(int vertex_amount) : parent_(std::vector<int>(vertex_amount)),
                                    weight_(std::vector<int>(vertex_amount, 0)),
                                    height_(std::vector<int>(vertex_amount, 1)) {
    for (int i = 0; i < vertex_amount; ++i) {
      parent_[i] = i;
    }
  }
  int FindSet(int vertex) {
    if (parent_[vertex] == vertex) {
      return vertex;
    }
    return parent_[vertex] = FindSet(parent_[vertex]);
  }

  void Unite(int vertex_1, int vertex_2, int weight) {
    int parent_1 = FindSet(vertex_1);
    int parent_2 = FindSet(vertex_2);
    if (parent_1 == parent_2) {
      weight_[parent_1] += weight;
      return;
    }
    if (height_[parent_1] == height_[parent_2]) {
      parent_[parent_2] = parent_1;
      weight_[parent_1] += weight_[parent_2] + weight;
      ++height_[parent_1];
    } else if (height_[parent_1] > height_[parent_2]) {
      parent_[parent_2] = parent_1;
      weight_[parent_1] += weight_[parent_2] + weight;
    } else {
      parent_[parent_1] = parent_2;
      weight_[parent_2] += weight_[parent_1] + weight;
    }
  }

  int GetComponentWeight(int vertex) {
    int parent = FindSet(vertex);
    return weight_[parent];
  }

 private:
  std::vector<int> parent_;
  std::vector<int> weight_;
  std::vector<int> height_;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int vertexes_amount = 0;
  std::cin >> vertexes_amount;
  int questions = 0;
  std::cin >> questions;
  DSU dsu(vertexes_amount);
  for (int i = 0; i < questions; ++i) {
    int command = 0;
    std::cin >> command;
    if (command == 1) {
      int vert_1, vert_2, weight;
      std::cin >> vert_1 >> vert_2 >> weight;
      --vert_1;
      --vert_2;
      dsu.Unite(vert_1, vert_2, weight);
    } else {
      int vert;
      std::cin >> vert;
      --vert;
      std::cout << dsu.GetComponentWeight(vert) << "\n";
    }
  }
}
