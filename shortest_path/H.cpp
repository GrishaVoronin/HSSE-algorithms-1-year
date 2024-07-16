#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

struct Parent {
  std::string parent;
  char move;
  int distance;
};

std::unordered_map<std::string, Parent> parent;

struct Field {
  Field() = default;

  Field(const std::vector<std::vector<int>>& field) : field_(field) {
    for (int line = 0; line < 3; ++line) {
      for (int column = 0; column < 3; ++column) {
        if (field[line][column] == 0) {
          zero_line_ = line;
          zero_column_ = column;
        }
      }
    }
    not_matching_cells_ = NotMatchingCells();
  }

  int NotMatchingCells() {
    int answer = 0;
    for (int line = 0; line < 3; ++line) {
      for (int column = 0; column < 3; ++column) {
        if (field_[line][column] != (line * 3 + column + 1) % 9) {
          ++answer;
        }
      }
    }
    return answer;
  }

  std::vector<Field> GetNeighbours() {
    std::vector<Field> neighbours;
    if (zero_line_ != 0) {
      std::vector<std::vector<int>> copy = field_;
      std::swap(copy[zero_line_][zero_column_], copy[zero_line_ - 1][zero_column_]);
      Field neighbour(copy);
      neighbour.move_made_ = move_made_ + 1;
      if (!parent.contains(neighbour.MakeStringOutOfField()) || parent[neighbour.MakeStringOutOfField()].distance > neighbour.move_made_ + neighbour.not_matching_cells_) {
        Parent cur_parent;
        cur_parent.parent = MakeStringOutOfField();
        cur_parent.distance = neighbour.move_made_ + neighbour.not_matching_cells_;
        cur_parent.move = 'U';
        parent[neighbour.MakeStringOutOfField()] = cur_parent;
        neighbours.push_back(neighbour);
      }
    }
    if (zero_line_ != 2) {
      std::vector<std::vector<int>> copy = field_;
      std::swap(copy[zero_line_][zero_column_], copy[zero_line_ + 1][zero_column_]);
      Field neighbour(copy);
      neighbour.move_made_ = move_made_ + 1;
      if (!parent.contains(neighbour.MakeStringOutOfField()) || parent[neighbour.MakeStringOutOfField()].distance > neighbour.move_made_ + neighbour.not_matching_cells_) {
        Parent cur_parent;
        cur_parent.parent = MakeStringOutOfField();
        cur_parent.distance = neighbour.move_made_ + neighbour.not_matching_cells_;
        cur_parent.move = 'D';
        parent[neighbour.MakeStringOutOfField()] = cur_parent;
        neighbours.push_back(neighbour);
      }
    }
    if (zero_column_ != 0) {
      std::vector<std::vector<int>> copy = field_;
      std::swap(copy[zero_line_][zero_column_], copy[zero_line_][zero_column_ - 1]);
      Field neighbour(copy);
      neighbour.move_made_ = move_made_ + 1;
      if (!parent.contains(neighbour.MakeStringOutOfField()) || parent[neighbour.MakeStringOutOfField()].distance > neighbour.move_made_ + neighbour.not_matching_cells_) {
        Parent cur_parent;
        cur_parent.parent = MakeStringOutOfField();
        cur_parent.distance = neighbour.move_made_ + neighbour.not_matching_cells_;
        cur_parent.move = 'L';
        parent[neighbour.MakeStringOutOfField()] = cur_parent;
        neighbours.push_back(neighbour);
      }
    }
    if (zero_column_ != 2) {
      std::vector<std::vector<int>> copy = field_;
      std::swap(copy[zero_line_][zero_column_], copy[zero_line_][zero_column_ + 1]);
      Field neighbour(copy);
      neighbour.move_made_ = move_made_ + 1;
      if (!parent.contains(neighbour.MakeStringOutOfField()) || parent[neighbour.MakeStringOutOfField()].distance > neighbour.move_made_ + neighbour.not_matching_cells_) {
        Parent cur_parent;
        cur_parent.parent = MakeStringOutOfField();
        cur_parent.distance = neighbour.move_made_ + neighbour.not_matching_cells_;
        cur_parent.move = 'R';
        parent[neighbour.MakeStringOutOfField()] = cur_parent;
        neighbours.push_back(neighbour);
      }
    }
    return neighbours;
  }

  bool operator==(const Field& other) const {
    return (field_ == other.field_);
  }

  std::string MakeStringOutOfField() {
    std::string answer = "";
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        answer += std::to_string(field_[i][j]);
      }
    }
    return answer;
  }

  std::vector<std::vector<int>> field_;
  int zero_line_;
  int zero_column_;
  int not_matching_cells_;
  int move_made_;
};


struct Cmp {
  bool operator () (const Field& field_1, const Field& field_2) const {
    return field_1.not_matching_cells_ + field_1.move_made_ >= field_2.not_matching_cells_ + field_2.move_made_;
  }
};

void PrintAnswer(std::string start, Field& finish_field) {
  std::cout << finish_field.move_made_ << "\n";
  std::vector<char> moves;
  std::string field = finish_field.MakeStringOutOfField();
  while (field != start) {
    moves.push_back(parent[field].move);
    field = parent[field].parent;
  }
  for (int i = moves.size() - 1; i >= 0; --i) {
    std::cout << moves[i];
  }
}

int main() {
  std::vector<std::vector<int>> start(3, std::vector<int>(3));
  std::string start_string;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      std::cin >> start[i][j];
      start_string += std::to_string(start[i][j]);
    }
  }
  Field start_field(start);
  start_field.move_made_ = 0;
  Field finish_field({{1, 2, 3}, {4, 5, 6}, {7, 8, 0}});
  std::unordered_map<std::string, bool> map;
  std::priority_queue<Field, std::vector<Field>, Cmp> priority_queue;
  priority_queue.push(start_field);
  while (!priority_queue.empty()) {
    Field cur_field = priority_queue.top();
    priority_queue.pop();
    if (map.contains(cur_field.MakeStringOutOfField())) {
      continue;
    }
    if (cur_field == finish_field) {
      PrintAnswer(start_string, cur_field);
      return 0;
    }
    map[cur_field.MakeStringOutOfField()] = true;
    for (Field& neighbour: cur_field.GetNeighbours()) {
      if (!map.contains(neighbour.MakeStringOutOfField())) {
        priority_queue.push(neighbour);
      }
    }
  }
  std::cout << -1;
}