#include <iostream>
#include <vector>
#include <stack>
#include <string>

struct Operation {
  bool is_cut;
  int first_vertex;
  int second_vertex;
};

class DSU{
 public:
  DSU(int vertex_amount) : parent_(std::vector<int>(vertex_amount)),
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

  bool AreSame(int vertex_1, int vertex_2) {
    return FindSet(vertex_1) == FindSet(vertex_2);
  }

  void Unite(int vertex_1, int vertex_2) {
    int parent_1 = FindSet(vertex_1);
    int parent_2 = FindSet(vertex_2);
    if (parent_1 == parent_2) {
      return;
    }
    if (height_[parent_1] == height_[parent_2]) {
      parent_[parent_2] = parent_1;
      ++height_[parent_1];
    } else if (height_[parent_1] > height_[parent_2]) {
      parent_[parent_2] = parent_1;
    } else {
      parent_[parent_1] = parent_2;
    }
  }

 private:
  std::vector<int> parent_;
  std::vector<int> height_;
};

int main() {
  int vertexes_amount, edges_amount, operations_amount;
  std::cin >> vertexes_amount >> edges_amount >> operations_amount;
  for (int i = 0; i < edges_amount; ++i) {
    int start, finish;
    std::cin >> start >> finish;
  }
  std::stack<Operation> operations;
  for (int i = 0; i < operations_amount; ++i) {
    std::string command;
    std::cin >> command;
    int start, finish;
    std::cin >> start >> finish;
    --start;
    --finish;
    if (command == "cut") {
      operations.push({true, start, finish});
    } else {
      operations.push({false, start, finish});
    }
  }
  DSU dsu(vertexes_amount);
  std::vector<std::string> answers;
  while (!operations.empty()) {
    Operation cur_operation = operations.top();
    operations.pop();
    if (cur_operation.is_cut) {
      dsu.Unite(cur_operation.first_vertex, cur_operation.second_vertex);
    } else {
      if (dsu.AreSame(cur_operation.first_vertex, cur_operation.second_vertex)) {
        answers.push_back("YES");
      } else {
        answers.push_back("NO");
      }
    }
  }
  for (auto iter = answers.rbegin(); iter != answers.rend(); ++iter) {
    std::cout << *iter << "\n";
  }
}